#pragma once

// Res folders.
#define S2D_TEXTURE_FOLDER "res/textures/"
#define S2D_SHADER_FOLDER  "res/shaders/"
#define S2D_FONTS_FOLDER   "res/fonts/"
#define S2D_ANIMATION_FILE "res/animations/animations.ani"


// Animation.
#define S2D_MAX_ANIMATION_NAME_LEN   32
#define S2D_MAX_ANIMATION_FRAMES     16
#define S2D_MAX_ANIMATION_TABLE_SIZE 1039


// Camera.
#define S2D_MAX_ZOOM 4000 
#define S2D_MIN_ZOOM 10


// Debug.
#define S2D_LOG_STATS_INTERVAL 1.0f // 1 second.


// ECS.
#define MAX_ENTITIES 10000


// Particles.
#define MAX_PARTICLES 10000


// Camera.
#define S2D_CAM_INITIAL_ZOOM 200.0f
#define S2D_CAM_UP ((clmVec3) { 0.0f, 1.0f, 0.0f })

// Fonts
#define S2D_MAX_FONTS 10
