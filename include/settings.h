#pragma once

// Resource locations.
#define S2D_TEXTURE_FOLDER          "res/textures/"
#define S2D_PARTICLE_SPRITES_FOLDER "res/textures/particles/"
#define S2D_SHADER_FOLDER           "res/shaders/"
#define S2D_FONTS_FOLDER            "res/fonts/"
#define S2D_ANIMATION_FILE          "res/animations/animations.ani"

// Animation.
#define S2D_MAX_ANIMATION_NAME_LEN   32   // character limit
#define S2D_MAX_ANIMATION_FRAMES     32   // frame count limit
#define S2D_MAX_ANIMATIONS           1000

// Camera.
#define S2D_MAX_ZOOM 4000 
#define S2D_MIN_ZOOM 10

// Debug.
#define S2D_LOG_STATS_INTERVAL 1.0f // 1 second.

// ECS.
#define S2D_MAX_ENTITIES 10000

// Particles.
#define S2D_MAX_PARTICLES 100000

// Camera.
#define S2D_CAM_INITIAL_ZOOM 200.0f
#define S2D_CAM_UP ((clmVec3) { 0.0f, 1.0f, 0.0f })

// Fonts
#define S2D_MAX_FONTS 10
