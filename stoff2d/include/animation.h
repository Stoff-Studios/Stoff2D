#pragma once

#include <settings.h>
#include <defines.h>

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

/* animations_get
 * --------------
 * Retrieve a reference to the animation with key: name.
 * Animations are loaded from res/animations/animations.ani
 * Look at README.ani to learn how to add animations in the above file.
 */
Animation* s2d_animations_get(char* name);

/* animations_init
 * ---------------
 * NOTE: This function is used by the engine in initialisation.
 *
 * Call this before anything else. This loads all the animations specified
 * in animations.ani which can be edited in the res/animations folder. 
 * 
 */
void s2d_animations_init();

/* animations_put
 * --------------
 * NOTE: This function is used by the engine in initialisation.
 *
 * Store animation with key: name.
 */
void s2d_animations_put(char* name, Animation animation);
