#pragma once

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>
#include <clm/clm.h>
#include <settings.h>


/********************************** Flags ************************************/

#define S2D_RUNNING   0x00000001
#define S2D_LOG_STATS 0x00000002

/*****************************************************************************/


/********************************* Keycodes **********************************/

// A-Z
#define S2D_KEY_A GLFW_KEY_A
#define S2D_KEY_B GLFW_KEY_B
#define S2D_KEY_C GLFW_KEY_C
#define S2D_KEY_D GLFW_KEY_D
#define S2D_KEY_E GLFW_KEY_E
#define S2D_KEY_F GLFW_KEY_F
#define S2D_KEY_G GLFW_KEY_G
#define S2D_KEY_H GLFW_KEY_H
#define S2D_KEY_I GLFW_KEY_I
#define S2D_KEY_J GLFW_KEY_J
#define S2D_KEY_K GLFW_KEY_K
#define S2D_KEY_L GLFW_KEY_L
#define S2D_KEY_M GLFW_KEY_M
#define S2D_KEY_N GLFW_KEY_N
#define S2D_KEY_O GLFW_KEY_O
#define S2D_KEY_P GLFW_KEY_P
#define S2D_KEY_Q GLFW_KEY_Q
#define S2D_KEY_R GLFW_KEY_R
#define S2D_KEY_S GLFW_KEY_S
#define S2D_KEY_T GLFW_KEY_T
#define S2D_KEY_U GLFW_KEY_U
#define S2D_KEY_V GLFW_KEY_V
#define S2D_KEY_W GLFW_KEY_W
#define S2D_KEY_X GLFW_KEY_X
#define S2D_KEY_Y GLFW_KEY_Y
#define S2D_KEY_Z GLFW_KEY_Z
// Arrow Keys
#define S2D_KEY_LEFT  GLFW_KEY_LEFT
#define S2D_KEY_RIGHT GLFW_KEY_RIGHT
#define S2D_KEY_UP    GLFW_KEY_UP
#define S2D_KEY_DOWN  GLFW_KEY_DOWN

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

// An Animation is a sequence of Frames which index into an array
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
