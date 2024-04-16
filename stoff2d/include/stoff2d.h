#pragma once

/* Welcome to the stoff2d engine!
 *
 * stoff2d is a C library for making 2D games.
 *
 * Property of Stoff-Studios pty ltd. You will be sued.
 */

#include <defines.h>         // Types, keycodes and flags.
#include <settings.h>        // Settings.
#include <clm/clm.h>         // Linear algebra.
#include <ecs.h>             // Entity Component System.
#include <sprite_renderer.h> // Layered sprite rendering.
#include <particle.h>        // Particle system.

/****************************** Init/Shutdown ********************************/

/* s2d_initialise_engine
 * ---------------------
 * Initialise the stoff2d engine.
 */
bool s2d_initialise_engine(const char* programName);


/* s2d_shutdown_engine
 * -------------------
 * Cleanup resources used by the engine.
 */
void s2d_shutdown_engine();

/*****************************************************************************/


/******************************** Rendering **********************************/

/* s2d_render_quad
 * ---------------
 * Render a textured quad.
 * For optimal performance, group render calls by texture.
 *
 * To render an entire texture, frame = { 0, 0, 1, 1 }
 */
void s2d_render_quad(
        clmVec2 position,
        clmVec2 size,
        clmVec4 colour,
        u32     texture,
        Frame   frame);

/* s2d_render_coloured_quad
 * ------------------------
 * Render a coloured quad.
 */
void s2d_render_coloured_quad(
        clmVec2 position,
        clmVec2 size,
        clmVec4 colour);

/* s2d_load_texture
 * ----------------
 * Returns a texture that can be used in s2d_render_quad. 
 * Textures must be png's located in S2D_TEXTURE_FOLDER.
 */
u32 s2d_load_texture(const char* fileName);

/*****************************************************************************/


/***************************** Start/End Frame *******************************/

/* s2d_start_frame
 * ---------------
 * Starts a new frame, returns timestep.
 */
f32 s2d_start_frame();

/* s2d_end_frame
 * -------------
 * Finish the current frame.
 */
void s2d_end_frame();

/*****************************************************************************/

/********************************** Flags ************************************/

/* s2d_check_flag
 * --------------
 * Returns true if flagsToCheck is on in the engine.
 * flagsToCheck can be one or more bitmasks or'd together (found in defines.h)
 */
bool s2d_check_flags(u32 flagsToCheck);


/* s2d_set_flags
 * ---------------
 * Turn on flagsToTurnOn in the engine.
 * flagsToTurnOn can be one or more bitmasks or'd together (found in defines.h)
 */
void s2d_set_flags(u32 flagsToTurnOn);

/* s2d_unset_flags
 * ---------------
 * Turn off flagsToTurnOff in the engine.
 * flagsToTurnOff can be one or more bitmasks or'd together (found in defines.h)
 */
void s2d_unset_flags(u32 flagsToTurnOff);

/*****************************************************************************/

/********************************* Window ************************************/

/* s2d_window_fullscreen
 * ---------------------
 * Make the window fullscreen.
 */
void s2d_window_fullscreen();

/* s2d_window_maximized
 * --------------------
 * Make the window windowed.
 */
void s2d_window_windowed();

/* s2d_keydown
 * -----------
 * Return true if the key is held down.
 * key code definitions are found in defines.h
 */
bool s2d_keydown(u32 key);

/* s2d_mouse_pos
 * -------------
 * Return the current position of the mouse in screen coordinates.
 */
clmVec2 s2d_mouse_pos();

/* s2d_mouse_world_pos
 * -------------------
 * Return the current position of the mouse in world coordinates.
 */
clmVec2 s2d_mouse_world_pos();

/*****************************************************************************/


/********************************** Camera ***********************************/

/* s2d_camera_get_pos
 * ------------------
 * Return the current position of the camera.
 */
clmVec2 s2d_camera_get_pos();

/* s2d_camera_set_pos
 * ------------------
 * Set the current position of the camera.
 */
void s2d_camera_set_pos(clmVec2 position);

/* s2d_camera_move
 * ---------------
 * Move the position of the camera by dx/dy along the x/y plane.
 */
void s2d_camera_move(f32 dx, f32 dy);

/* s2d_camera_zoom
 * ---------------
 * Zoom the camera by dz. Increasing zoom zooms out.
 */
void s2d_camera_zoom(f32 dz);

/* s2d_camera_zoom
 * ---------------
 * Set the zoom level to z.
 */
void s2d_camera_set_zoom(f32 z);

/*****************************************************************************/


/******************************* Animation ***********************************/

/* animations_get
 * --------------
 * Retrieve a reference to the animation with key: name.
 * Animations are loaded from res/animations/animations.ani
 * Look at README.ani to learn how to add animations in the above file.
 */
Animation* s2d_animations_get(char* name);

/*****************************************************************************/
