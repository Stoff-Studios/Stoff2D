#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stoff2d_core.h>
#include <shader.h>
#include <stbi/stbi_image.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    // Renderer
    u32         vao;
    u32         vbo;
    u32         ebo;
    u32         quadShader;
    Vertex      vertices[S2D_MAX_VERTICES];
    Vertex*     currentVertex;
    u32         indicesCount;
    u32         verticesCount;
    u32         lastTexture;
    u32         whiteTex;

    // Time
    f64         lastTime;
    f32         timeStep;

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
void renderer_init();
u32  load_texture(const char* fileName);
void render_flush();

// Camera.
void    camera_init();
clmMat4 camera_view();
clmMat4 camera_projection();

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
    engine.lastTime = glfwGetTime();
    engine.timeStep = 0.0f;

    // Debugging.
    engine.logStatsTimer = 0;
    engine.drawCalls     = 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
    glfwSwapInterval(S2D_VSYNC);

    // Initialize OpenGL function pointers with GLAD.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "[S2D ERROR] GLAD failed to initialise\n");
        return false;
    }

    glViewport(0, 0, engine.winWidth, engine.winHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set Callbacks.
    glfwSetFramebufferSizeCallback(engine.winPtr, framebuffer_size_callback);

    // Initialise Renderer.
    renderer_init();

    // Load animations.
    animations_init();

    // Setup the sprite renderer.
    sprite_renderer_init();

    // Initialise particle system.
    particles_init();

    // Load white texture for coloured quads.
    engine.whiteTex = s2d_load_texture("white.png");

    // All is well so start the application.
    engine.flags = S2D_RUNNING;

    return true;
}

f32 s2d_start_frame() {
    // Obtain delta-time.
    f64 currentTime = glfwGetTime();
    engine.timeStep = (f32) (currentTime - engine.lastTime);
    engine.lastTime = currentTime;

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

    // Update projection and view transforms in the shader.
    shader_use(engine.quadShader);

    // Projection (ortho)
    shader_set_uniform_mat4(
            engine.quadShader,
            "proj",
            camera_projection());

    // View (lookat)
    shader_set_uniform_mat4(
            engine.quadShader,
            "view",
            camera_view());

    particles_update(engine.timeStep);

    return engine.timeStep;
}

void s2d_end_frame() {
    render_flush();
    glfwSwapBuffers(engine.winPtr);

    // Log stats.
    if (s2d_check_flags(S2D_LOG_STATS) &&
            engine.logStatsTimer > S2D_LOG_STATS_INTERVAL) {
        log_stats();
        engine.logStatsTimer = 0.0f;
    }
}

void render_flush() {
    // Set shader, bind vao, fill vbo.
    shader_use(engine.quadShader);
    glBindTexture(GL_TEXTURE_2D, engine.lastTexture);
    glBindVertexArray(engine.vao);
    glBindVertexArray(engine.vbo);
    glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            engine.verticesCount * sizeof(Vertex),
            engine.vertices);

    // Render draw call.
    glDrawElements(GL_TRIANGLES, engine.indicesCount, GL_UNSIGNED_INT, 0);

    // Reset for next batch.
    engine.verticesCount = 0;
    engine.indicesCount  = 0;
    engine.currentVertex = engine.vertices;

    // Stats.
    engine.drawCalls++;
}

void s2d_render_quad(
        clmVec2 position, 
        clmVec2 size, 
        clmVec4 colour,
        u32     texture,  
        Frame   frame) {
    // Flush upon filling up the buffer or texture change.
    if (engine.verticesCount == S2D_MAX_VERTICES ||
            engine.lastTexture != texture) {
        render_flush();
    }

    // Bottom-left.
    engine.currentVertex->position.x = position.x;
    engine.currentVertex->position.y = position.y;
    engine.currentVertex->texCoord.x = frame.x;
    engine.currentVertex->texCoord.y = frame.y;
    engine.currentVertex->colour = colour;
    engine.currentVertex++;

    // Bottom-right.
    engine.currentVertex->position.x = position.x + size.x;
    engine.currentVertex->position.y = position.y;
    engine.currentVertex->texCoord.x = frame.x + frame.w;
    engine.currentVertex->texCoord.y = frame.y;
    engine.currentVertex->colour = colour;
    engine.currentVertex++;

    // Top-right.
    engine.currentVertex->position.x = position.x + size.x;
    engine.currentVertex->position.y = position.y + size.y;
    engine.currentVertex->texCoord.x = frame.x + frame.w;
    engine.currentVertex->texCoord.y = frame.y + frame.h;
    engine.currentVertex->colour = colour;
    engine.currentVertex++;

    // Top-left.
    engine.currentVertex->position.x = position.x;
    engine.currentVertex->position.y = position.y + size.y;
    engine.currentVertex->texCoord.x = frame.x;
    engine.currentVertex->texCoord.y = frame.y + frame.h;
    engine.currentVertex->colour = colour;
    engine.currentVertex++;

    // Increment our counts and update the last added texture.
    engine.indicesCount  += 6;
    engine.verticesCount += 4;
    engine.lastTexture = texture;
}

void s2d_render_coloured_quad(
        clmVec2 position,
        clmVec2 size,
        clmVec4 colour) {
    s2d_render_quad(
            position,
            size,
            colour,
            engine.whiteTex,
            (Frame) {  0.0f, 0.0f, 1.0f, 1.0f });
}

void s2d_shutdown_engine() {
    glfwDestroyWindow(engine.winPtr);
    sprite_renderer_shutdown();
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

void renderer_init() {
    // vao
    glGenVertexArrays(1, &engine.vao); 
    glBindVertexArray(engine.vao);

    // vbo
    glGenBuffers(1, &engine.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, engine.vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            S2D_MAX_VERTICES * sizeof(Vertex),
            NULL,
            GL_DYNAMIC_DRAW);

    // attribute 0, position
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(
            0,                                   // attribute no.
            2,                                   // number of elements
            GL_FLOAT,                            // data type of the elements
            GL_FALSE,                            // normalise
            sizeof(Vertex),                      // stride
            (void*) offsetof(Vertex, position)); // offset
    // attribute 1, texCoords
    glEnableVertexAttribArray(1);  
    glVertexAttribPointer(
            1,                                    // attribute no.
            2,                                    // number of elements
            GL_FLOAT,                             // data type of the elements
            GL_FALSE,                             // normalise
            sizeof(Vertex),                       // stride
            (void*) offsetof(Vertex, texCoord));  // offset
    // attribute 2, colour
    glEnableVertexAttribArray(2);  
    glVertexAttribPointer(
            2,                                    // attribute no.
            4,                                    // number of elements
            GL_FLOAT,                             // data type of the elements
            GL_FALSE,                             // normalise
            sizeof(Vertex),                       // stride
            (void*) offsetof(Vertex, colour));  // offset

    // ebo
    u32* indices = (u32*) malloc(S2D_MAX_INDICES * sizeof(u32));
    u32 offset = 0;
    for (u32 i = 0; i < S2D_MAX_INDICES; i += 6 ) {
        indices[i + 0] = offset + 0; 
        indices[i + 1] = offset + 1; 
        indices[i + 2] = offset + 2; 
        indices[i + 3] = offset + 2; 
        indices[i + 4] = offset + 3; 
        indices[i + 5] = offset + 0; 
        offset += 4;
    }
    glGenBuffers(1, &engine.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine.ebo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            S2D_MAX_INDICES * sizeof(u32),
            indices, 
            GL_STATIC_DRAW); 
    free(indices);

    // Unbind.
    glBindVertexArray(0);

    // Load shaders.
    engine.quadShader = shader_create("vQuad.glsl", "fQuad.glsl");

    // Batch renderer stuff.
    engine.currentVertex = engine.vertices;
    engine.verticesCount = 0;
    engine.indicesCount  = 0;
    engine.lastTexture   = 0x80808080; // garbage number.
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

void s2d_clear_colour(clmVec4 colour) {
    glClearColor(colour.r, colour.g, colour.b, colour.a);
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

/*****************************************************************************/


/******************************* Callbacks ***********************************/

void framebuffer_size_callback(GLFWwindow* winPtr, i32 width, i32 height) {
    glViewport(0, 0, width, height);
    engine.winWidth  = width;
    engine.winHeight = height;
    engine.aspectRatio = ((f32) engine.winWidth) / ((f32) engine.winHeight);
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

clmMat4 camera_projection() {
    return clm_mat4_ortho(
             -engine.camZoom * engine.aspectRatio,
              engine.camZoom * engine.aspectRatio,
             -engine.camZoom,
              engine.camZoom,
              0.1f,
              100.0f);
}

clmMat4 camera_view() {
    return clm_mat4_lookat(
            engine.camPos,
            (clmVec3) { engine.camPos.x, engine.camPos.y, -1.0f },
            engine.camUp);
}

/*****************************************************************************/