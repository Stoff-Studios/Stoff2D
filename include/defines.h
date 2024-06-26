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
} Vertex;

/*****************************************************************************/


/******************************** Animation **********************************/

// A Frame is a normalised rect region of a texture.
typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} Frame;

// An Animation is a sequence of Frames
typedef struct {
    Frame frames[S2D_MAX_ANIMATION_FRAMES];
    u32   frameCount;
} Animation;

// For sprite renderer.
typedef struct {
    clmVec2 position;
    clmVec2 size;
    clmVec4 colour;
    u32     texture;
    Frame   frame;
    u8      layer;
    u32     shader;
} s2dSprite;

/*****************************************************************************/


/******************************** Particles **********************************/

typedef struct {
    u32     count;         // number of particles to spawn.
    f32     lifeTime;      // lifetime of each particle in seconds.
    clmVec2 position;
    clmVec2 lowerVelocity; // lower velocity bound.
    clmVec2 upperVelocity; // upper velocity bound.
    clmVec2 lowerSize;     // lower size bound.
    clmVec2 upperSize;     // upper size bound.
    clmVec4 birthColour;   // start colour.
    clmVec4 deathColour;   // end colour.
} ParticleData;

/*****************************************************************************/

/***************************** Font and Text *********************************/


/*****************************************************************************/
