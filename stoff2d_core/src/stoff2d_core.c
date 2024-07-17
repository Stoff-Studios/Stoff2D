#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stoff2d_core.h>

#include <font.h>
#include <utils.h>
#include <quad_renderer.h>
#include <stbi/stbi_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
    // Internal flags
    u32         flags;

    // Window
    GLFWwindow*  winPtr;
    GLFWmonitor* monitor;
    i32          scrWidth;
    i32          scrHeight;
    i32          refreshRate;
    i32          winWidth;
    i32          winHeight;
    f32          aspectRatio;

    // Renderer and default shaders
    QuadRenderer quadRenderer;
    u32          quadShader;
    u32          textShader;

    // Time
    f64         lastTime;
    f32         timeStep;
    f32         targetFrameTime;

    // Camera.
    f32     camZoom;
    clmVec3 camPos;
    clmVec3 camUp;

    // Debugging.
    f32 logStatsTimer;
    u32 drawCalls;
} EngineData;

EngineData engine;

/*************************** Forward Declerations ****************************/

// Stats logging.
void log_stats();

// Callbacks.
void framebuffer_size_callback(GLFWwindow* winPtr, i32 width, i32 height);

// Renderer.
u32  load_texture(const char* fileName);
clmMat4 text_projection();

// Camera.
void camera_init();

// Animation.
void animations_init();

// Particle System.
void particles_init();
void particles_update(f32 timeStep);

// Sprite Renderer.
void sprite_renderer_init();
void sprite_renderer_shutdown();

/*****************************************************************************/


// Initialise engine.
bool s2d_initialise_engine(const char* programName) {
    // Initialise glfw.
    glfwInit();

    // Flags.
    engine.flags = 0;

    // Window
    engine.monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(engine.monitor);
    engine.scrWidth    = mode->width;
    engine.scrHeight   = mode->height;
    engine.winWidth    = mode->width;
    engine.winHeight   = mode->height;
    engine.refreshRate = mode->refreshRate;
    engine.aspectRatio = ((f32) engine.winWidth) / ((f32) engine.winHeight);

    // Camera.
    camera_init();

    // Time.
    engine.lastTime        = glfwGetTime();
    engine.timeStep        = 0.0f;
    engine.targetFrameTime = 0.0f; // uncapped.

    // Debugging.
    engine.logStatsTimer = 0;
    engine.drawCalls     = 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RED_BITS,     mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS,   mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS,    mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // Create our window.
    engine.winPtr = glfwCreateWindow(
            engine.winWidth, 
            engine.winHeight,
            programName, 
            engine.monitor,
            NULL);
    if (!engine.winPtr) {
        fprintf(stderr, "[S2D ERROR] failed to create window\n");
        return false;
    }

    glfwMakeContextCurrent(engine.winPtr);

    // Initialize OpenGL function pointers with GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "[S2D ERROR] GLAD failed to initialise\n");
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialise fonts.
    if (!font_init()) {
        return false;
    }

    // Set Callbacks.
    glfwSetFramebufferSizeCallback(engine.winPtr, framebuffer_size_callback);

    engine.quadRenderer = quad_renderer_create();
    engine.quadShader = s2d_shader_create(
            "engine/vQuad.glsl", "engine/fQuad.glsl");
    engine.textShader = s2d_shader_create(
            "engine/vText.glsl", "engine/fQuad.glsl");
    s2d_shader_set_uniform_mat4(
            engine.textShader,
            "proj",
            text_projection());

    animations_init();
    sprite_renderer_init();
    particles_init();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, engine.winWidth, engine.winHeight);

    engine.flags = S2D_RUNNING;

    return true;
}

f32 s2d_start_frame() {
    // Obtain delta-time.
    f64 currentTime = glfwGetTime();
    engine.timeStep = (f32) (currentTime - engine.lastTime);
    engine.lastTime = currentTime;

    // Limit fps to framecap.
    if (engine.timeStep < engine.targetFrameTime) {
        f32 sleepTime = engine.targetFrameTime - engine.timeStep;
        utils_sleep(sleepTime);
        engine.timeStep += sleepTime;
        engine.lastTime += sleepTime;
    }

    if (s2d_check_flags(S2D_PAUSED)) {
        engine.timeStep = 0.0f;
    }

    // Stats.
    engine.logStatsTimer += engine.timeStep;
    engine.drawCalls = 0;

    // Clear screen and poll events.
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    // Update internal state.
    glfwGetWindowSize(engine.winPtr, &engine.winWidth, &engine.winHeight);
    if (glfwWindowShouldClose(engine.winPtr)) {
        s2d_unset_flags(S2D_RUNNING);
    }

    // update default quad shader view and projection
    // (text shader only needs updating on window resize.
    s2d_shader_set_uniform_mat4(
            engine.quadShader,
            "proj",
            s2d_camera_projection());
    s2d_shader_set_uniform_mat4(
            engine.quadShader,
            "view",
            s2d_camera_view());

    particles_update(engine.timeStep);

    return engine.timeStep;
}

void s2d_end_frame() {
    quad_renderer_flush(engine.quadRenderer);
    glfwSwapBuffers(engine.winPtr);

    // Log stats.
    if (s2d_check_flags(S2D_LOG_STATS) &&
            engine.logStatsTimer > S2D_LOG_STATS_INTERVAL) {
        quad_renderer_print_stats(engine.quadRenderer);
        engine.logStatsTimer = 0.0f;
    }

    quad_renderer_reset_stats(engine.quadRenderer);
}

void s2d_set_frame_cap(u32 fps) {
    switch (fps) {
        case S2D_FPS_UNCAPPED:
            glfwSwapInterval(0);
            engine.targetFrameTime = 0.0f;
            break;
        case S2D_FPS_VSYNC:
            glfwSwapInterval(1);
            break;
        default:
            glfwSwapInterval(0);
            engine.targetFrameTime = 1.0f / (f32) fps;
    }
}

/****************************** Rendering ************************************/

void s2d_render_flush() {
    quad_renderer_flush(engine.quadRenderer);
}

void s2d_render_quad(
        clmVec2  position,
        clmVec2  size,
        clmVec4  colour,
        u32      texture,
        s2dFrame frame,
        u32      shader) {
    quad_renderer_submit_quad(
            engine.quadRenderer, position, size, colour,
            texture, frame, shader);
}

u32 s2d_get_quad_shader() {
    return engine.quadShader;
}

u32 s2d_get_text_shader() {
    return engine.textShader;
}

void s2d_shutdown_engine() {
    glfwDestroyWindow(engine.winPtr);
    quad_renderer_shutdown(engine.quadRenderer);
    sprite_renderer_shutdown();
    font_shutdown();
    glfwTerminate();
}

bool s2d_check_flags(u32 flagsToCheck) {
    return engine.flags & flagsToCheck;
}

void s2d_set_flags(u32 flagsToTurnOn) {
    engine.flags |= flagsToTurnOn;
}

void s2d_unset_flags(u32 flagsToTurnOff) {
    engine.flags &= 0xffffffff ^ flagsToTurnOff;
}

u32 s2d_load_texture(const char* fileName) {
    // Construct path to texture.
    const char* textureDir = S2D_TEXTURE_FOLDER;
    size_t texPathLen = strlen(textureDir) + strlen(fileName);
    char* texPath = (char*) malloc((texPathLen * sizeof(char)) + 1);
    strcpy(texPath, textureDir);
    strcat(texPath, fileName);

    // Load texture data with stbi.
    stbi_set_flip_vertically_on_load(true);
    i32 width, height, nrChannels;
    u8* data = stbi_load(texPath, &width, &height, &nrChannels, 0); 

    // Create OpenGL texture.
    u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);  

    // Settings, could change these if needed.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data) {
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "failed to load image %s\n", texPath);
    }

    stbi_image_free(data);
    free(texPath);

    return texture;
}

void s2d_set_texture_slot(u32 slot, u32 texID) {
    quad_renderer_set_texture_slot(engine.quadRenderer, slot, texID);
}

void s2d_reset_texture_slots() {
    quad_renderer_reset_texture_slots(engine.quadRenderer);
}

void s2d_clear_colour(clmVec4 colour) {
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void s2d_clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/********************************* Window ************************************/

bool s2d_keydown(u32 key) {
    return glfwGetKey(engine.winPtr, key) == GLFW_PRESS;
}

clmVec2 s2d_mouse_screen_pos() {
    double x, y;
    glfwGetCursorPos(engine.winPtr, &x, &y);
    return (clmVec2) { x, engine.winHeight - y };
}

clmVec2 s2d_mouse_world_pos() {
    clmVec2 bottomLeft = (clmVec2) {
        .x = engine.camPos.x - (engine.camZoom * engine.aspectRatio),
        .y = engine.camPos.y - engine.camZoom
    };
    clmVec2 mousePos = s2d_mouse_screen_pos();
    // Normalise the position of the mouse cursor in screen space, then multiply
    // that by the width of the screen and add that to the bottom left world space.
    return (clmVec2) {
        .x = bottomLeft.x + (2.0f * engine.camZoom * engine.aspectRatio * (mousePos.x / ((f32) engine.winWidth))),
        .y = bottomLeft.y + (2.0f * engine.camZoom * (mousePos.y / ((f32) engine.winHeight)))
    };
}

void s2d_window_fullscreen() {
    glfwSetWindowMonitor(
            engine.winPtr,
            engine.monitor, 
            0, 0, 
            engine.scrWidth, engine.scrHeight, 
            engine.refreshRate);
}

void s2d_window_windowed() {
    glfwSetWindowMonitor(
            engine.winPtr,
            NULL, 
            engine.scrWidth / 4, engine.scrHeight / 4, 
            engine.scrWidth / 2, engine.scrHeight / 2, 
            engine.refreshRate);
}

clmVec4 s2d_get_screen_rect() {
    clmVec2 camPos = s2d_camera_get_pos();
    f32 viewWidth  = 2.0f * engine.camZoom * engine.aspectRatio;
    f32 viewHeight = 2.0f * engine.camZoom;
    return (clmVec4) {
        camPos.x - (viewWidth * 0.5f),
        camPos.y - (viewHeight * 0.5f),
        viewWidth,
        viewHeight
    };
}

clmVec2 s2d_get_viewport_dimensions() {
    return (clmVec2) { engine.winWidth, engine.winHeight };
}

clmVec2 s2d_get_screen_dimensions() {
    return (clmVec2) { engine.scrWidth, engine.scrHeight };
}

void s2d_rendertexture_set_target_screen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, engine.winWidth, engine.winHeight);
}

void s2d_set_blend_mode(s2dBlendMode blendMode) {
    switch (blendMode) {
        case BLEND_MODE_RENDER_TO_SCREEN:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BLEND_MODE_RENDER_TEXTURE_TO_SCREEN:
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        case BLEND_MODE_RENDER_TO_TEXTURE:
            glBlendFuncSeparate(
                    GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 
                    GL_ONE_MINUS_DST_ALPHA, GL_ONE);
            break;
    }
}

/*****************************************************************************/


/******************************* Callbacks ***********************************/

void framebuffer_size_callback(GLFWwindow* winPtr, i32 width, i32 height) {
    glViewport(0, 0, width, height);
    engine.winWidth  = width;
    engine.winHeight = height;
    engine.aspectRatio = ((f32) engine.winWidth) / ((f32) engine.winHeight);
    s2d_shader_set_uniform_mat4(
            engine.textShader,
            "proj",
            text_projection());
}

/*****************************************************************************/


/*********************************** Misc ************************************/

void log_stats() {
    printf("--- STATS ---\n"
           "fps:   %.2f\n"
           "draws: %u\n"
           "-------------\n",
           1.0f / engine.timeStep,
           engine.drawCalls);
}

/*****************************************************************************/


/********************************** Camera ***********************************/

clmVec2 s2d_camera_get_pos() {
    return (clmVec2) { engine.camPos.x, engine.camPos.y };
}

void s2d_camera_set_pos(clmVec2 position) {
    engine.camPos.x = position.x;
    engine.camPos.y = position.y;
}

void s2d_camera_move(f32 dx, f32 dy) {
    engine.camPos.x += dx;
    engine.camPos.y += dy;
}

void s2d_camera_zoom(f32 dz) {
    engine.camZoom += dz;
    if (engine.camZoom > S2D_MAX_ZOOM) {
        engine.camZoom = S2D_MAX_ZOOM;
    } else if (engine.camZoom < S2D_MIN_ZOOM) {
        engine.camZoom = S2D_MIN_ZOOM;
    }
}

void s2d_camera_set_zoom(f32 z) {
    engine.camZoom = z;
}

void camera_init() {
    engine.camPos  = (clmVec3) { 0.0f, 0.0f, 1.0f };
    engine.camZoom = S2D_CAM_INITIAL_ZOOM;
    engine.camUp   = S2D_CAM_UP;
}

clmMat4 s2d_camera_projection() {
    return clm_mat4_ortho(
             -engine.camZoom * engine.aspectRatio,
              engine.camZoom * engine.aspectRatio,
             -engine.camZoom,
              engine.camZoom,
              0.1f,
              100.0f);
}

clmMat4 text_projection() {
    return clm_mat4_ortho(
            0,
            engine.winWidth,
            0,
            engine.winHeight,
            0.0f,
            100.0f);
}

clmMat4 s2d_camera_view() {
    return clm_mat4_lookat(
            engine.camPos,
            (clmVec3) { engine.camPos.x, engine.camPos.y, -1.0f },
            engine.camUp);
}

/*****************************************************************************/



/**************************** Font and Text **********************************/

void s2d_text_render_bitmap(
        const char* fontName,
        clmVec2     position,
        clmVec2     size,
        clmVec4     colour) {
    // query font.
    s2dFont* font;
    if (!(font = font_get_font(fontName))) {
        fprintf(stderr, "[S2D Warning] tried to render text with non-existent "
                "font - %s\n", fontName);
        return;
    }

    s2d_render_quad(
            position,
            size,
            colour,
            font->fontTexID,
            (s2dFrame) { 0.0f, 0.0f, 1.0f, 1.0f },
            engine.quadShader);
}

void s2d_text_render(
        const char* fontName,
        clmVec2     position,
        clmVec4     colour,
        f32         scale,
        u32         layer,
        u32         shader,
        const char* formatText,
        ...) {
    // query font.
    s2dFont* font;
    if (!(font = font_get_font(fontName))) {
        fprintf(stderr, "[S2D Warning] tried to render text with non-existent "
                "font - %s\n", fontName);
        return;
    }

    // format string with args.
    char buffer[4096];
    va_list aptr;
    va_start(aptr, formatText);
    int result = vsprintf(buffer, formatText, aptr);
    va_end(aptr);

    if (result < 0) {
        fprintf(stderr, 
                "[S2D Warning] could not format string when rendering text\n");
    }

    // now render each charater using sprite renderer.
    for (u32 i = 0; i < strlen(buffer); i++) {
        if (buffer[i] > 127) {
            fprintf(stderr,
                    "[S2D Error] character \'%c\' is above ASCII limit 127, "
                    "skipped rendering\n",
                    buffer[i]);
            continue;
        }
        s2dChar c = font->chars[buffer[i]];
        s2dSprite glyphSprite = (s2dSprite) {
            .position = (clmVec2) { 
                .x = position.x + (c.bearingX * scale), 
                .y = position.y + ((c.bearingY - c.height) * scale)
            },
            .size    = (clmVec2) { scale * c.width, scale * c.height },
            .texture = font->fontTexID,
            .colour  = colour,
            .frame   = c.texRegion,
            .layer   = layer,
            .shader  = shader
        };
        s2d_sprite_renderer_add_sprite(glyphSprite);
        position.x += (c.advance >> 6) * scale;
    }
}

/*****************************************************************************/

