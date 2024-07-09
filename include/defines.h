#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <clm/clm.h>
#include <settings.h>

#define S2D_FPS_UNCAPPED 0xffffffff
#define S2D_FPS_VSYNC    0xfffffff0

/********************************** Flags ************************************/

#define S2D_RUNNING   0x00000001
#define S2D_LOG_STATS 0x00000002
#define S2D_PAUSED    0x00000004

/*****************************************************************************/


/*********************************** Misc ************************************/

#define S2D_ENTIRE_TEXTURE ((s2dFrame) { 0.0f, 0.0f, 1.0f, 1.0f })

/*****************************************************************************/


/********************************* Keycodes **********************************/

// A-Z
#define S2D_KEY_A 65
#define S2D_KEY_B 66
#define S2D_KEY_C 67
#define S2D_KEY_D 68
#define S2D_KEY_E 69
#define S2D_KEY_F 70
#define S2D_KEY_G 71
#define S2D_KEY_H 72
#define S2D_KEY_I 73
#define S2D_KEY_J 74
#define S2D_KEY_K 75
#define S2D_KEY_L 76
#define S2D_KEY_M 77
#define S2D_KEY_N 78
#define S2D_KEY_O 79
#define S2D_KEY_P 80
#define S2D_KEY_Q 81
#define S2D_KEY_R 82
#define S2D_KEY_S 83
#define S2D_KEY_T 84
#define S2D_KEY_U 85
#define S2D_KEY_V 86
#define S2D_KEY_W 87
#define S2D_KEY_X 88
#define S2D_KEY_Y 89
#define S2D_KEY_Z 90
// Arrow Keys
#define S2D_KEY_RIGHT 262
#define S2D_KEY_LEFT  263
#define S2D_KEY_DOWN  264
#define S2D_KEY_UP    265

/*****************************************************************************/


/******************************** Base types *********************************/

// Unsigned types.
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


// Signed types.
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


// Floating point types.
typedef float  f32;
typedef double f64;

/*****************************************************************************/


/********************************* Renderer **********************************/

#define S2D_COLOURED_QUAD_TEXTURE 0x8080

#define S2D_MAX_QUADS    10000 
#define S2D_MAX_VERTICES (4 * S2D_MAX_QUADS)
#define S2D_MAX_INDICES  (6 * S2D_MAX_QUADS)

typedef struct {
    clmVec2 position;
    clmVec2 texCoord;
    clmVec4 colour;
} s2dVertex;

typedef struct {
    u32 frameBufferID;
    u32 textureID;
    u32 width;
    u32 height;
} s2dRenderTexture;

typedef enum {
    BLEND_MODE_RENDER_TO_TEXTURE,
    BLEND_MODE_RENDER_TEXTURE_TO_SCREEN,
    BLEND_MODE_RENDER_TO_SCREEN
} s2dBlendMode;

/*****************************************************************************/


/******************************** Animation **********************************/

// A Frame is a normalised rect region of a texture.
typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} s2dFrame;

// An Animation is a sequence of Frames
typedef struct {
    s2dFrame frames[S2D_MAX_ANIMATION_FRAMES];
    u32      frameCount;
} s2dAnimation;

// For sprite renderer.
typedef struct {
    clmVec2  position;
    clmVec2  size;
    clmVec4  colour;
    u32      texture;
    s2dFrame frame;
    u8       layer;
    u32      shader;
} s2dSprite;

/*****************************************************************************/


/******************************** Particles **********************************/

typedef struct {
    u32         count;         // number of particles to spawn per add call.
    f32         lifeTime;      // lifetime of each particle in seconds.
    clmVec2     lowerVelocity; // lower bound for birth velocity.
    clmVec2     upperVelocity; // upper bound for birth velocity.
    u32         lowerSize;     // lower bound for birth size, in pixels.
    u32         upperSize;     // upper bound for birth size, in pixels.
    clmVec4     birthColour;   // birth colour.
    clmVec4     deathColour;   // death colour.
    u32         shader;        // shader program to render particles with.
    const char* spriteName;    // png file name in S2D_PARTICLE_SPRITES_FOLDER
                               // (not including .png)
} s2dParticleType;

/*****************************************************************************/
