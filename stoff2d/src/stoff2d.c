#include <glad/glad.h>

#include <stoff2d.h>
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

    // Camera
    f32         camZoom;
    clmVec3     camPos;
    clmVec3     camUp;

    // Time
    f64         lastTime;
    f32         timeStep;

    // Debugging.
    f32 logStatsTimer;
    u32 drawCalls;
} s2d_Engine;

static s2d_Engine engine;

// Stats logging.
void log_stats();

// Callbacks
void framebuffer_size_callback(GLFWwindow* winPtr, i32 width, i32 height);

// Renderer helpers.
void initialise_renderer();
u32  load_texture(const char* fileName);
void render_flush();

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
    engine.camZoom  = 200.0f;
    engine.camPos   = (clmVec3) { 0.0f, 0.0f, 1.0f };
    engine.camUp    = (clmVec3) { 0.0f, 1.0f, 0.0f };

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Initialize Renderer.
    initialise_renderer();

    // Set Callbacks.
    glfwSetFramebufferSizeCallback(engine.winPtr, framebuffer_size_callback);

    // Load animations from animations.ani
    s2d_animations_init();

    // Setup the sprite renderer.
    sprite_renderer_init();

    // Initialise the ecs.
    ecs_init();

    // All is well so start the application.
    engine.flags = S2D_RUNNING;

    return true;
}

f32 s2d_start_frame() {
    // Obtain delta-time.
    f64 currentTime = glfwGetTime();
    engine.timeStep = (f32) (currentTime - engine.lastTime);
    engine.lastTime = currentTime;

    // Stats.
    engine.logStatsTimer += engine.timeStep;
    engine.drawCalls = 0;

    // Clear screen and poll events.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
            clm_mat4_ortho(
               -engine.camZoom * engine.aspectRatio,
                engine.camZoom * engine.aspectRatio,
               -engine.camZoom,
                engine.camZoom,
                0.1f,
                100.0f));

    // View (lookat)
    shader_set_uniform_mat4(
            engine.quadShader,
            "view",
            clm_mat4_lookat(
                engine.camPos,
                (clmVec3) { engine.camPos.x, engine.camPos.y, -1.0f },
                engine.camUp));

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
    engine.currentVertex++;

    // Bottom-right.
    engine.currentVertex->position.x = position.x + size.x;
    engine.currentVertex->position.y = position.y;
    engine.currentVertex->texCoord.x = frame.x + frame.w;
    engine.currentVertex->texCoord.y = frame.y;
    engine.currentVertex++;

    // Top-right.
    engine.currentVertex->position.x = position.x + size.x;
    engine.currentVertex->position.y = position.y + size.y;
    engine.currentVertex->texCoord.x = frame.x + frame.w;
    engine.currentVertex->texCoord.y = frame.y + frame.h;
    engine.currentVertex++;

    // Top-left.
    engine.currentVertex->position.x = position.x;
    engine.currentVertex->position.y = position.y + size.y;
    engine.currentVertex->texCoord.x = frame.x;
    engine.currentVertex->texCoord.y = frame.y + frame.h;
    engine.currentVertex++;

    // Increment our counts and update the last added texture.
    engine.indicesCount  += 6;
    engine.verticesCount += 4;
    engine.lastTexture = texture;
}

void s2d_shutdown_engine() {
    glfwDestroyWindow(engine.winPtr);
    sprite_renderer_shutdown();
    ecs_shutdown();
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



void initialise_renderer() {
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

    // Unbind. Could leave bound since we are only using one buffer.
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Load shaders.
    engine.quadShader = shader_create("vQuad.glsl", "fQuad.glsl");

    // Make sure the uniform sampler corresponds to correct slot.
    shader_set_uniform_1i(engine.quadShader, "spriteSheet", 0);
    glActiveTexture(GL_TEXTURE0);

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


/********************************* Camera ************************************/

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

clmVec2 s2d_camera_get_pos() {
    return (clmVec2) { engine.camPos.x, engine.camPos.y };
}

void s2d_camera_set_pos(clmVec2 position) {
    engine.camPos.x = position.x;
    engine.camPos.y = position.y;
}


/********************************* Window ************************************/

bool s2d_keydown(u32 key) {
    return glfwGetKey(engine.winPtr, key) == GLFW_PRESS;
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


/******************************* Callbacks ***********************************/

void framebuffer_size_callback(GLFWwindow* winPtr, i32 width, i32 height) {
    glViewport(0, 0, width, height);
    engine.winWidth  = width;
    engine.winHeight = height;
    engine.aspectRatio = ((f32) engine.winWidth) / ((f32) engine.winHeight);
}

/********************************* Misc **************************************/

void log_stats() {
    printf("--- STATS ---\n"
           "fps:   %.2f\n"
           "draws: %u\n"
           "-------------\n",
           1.0f / engine.timeStep,
           engine.drawCalls);
}
