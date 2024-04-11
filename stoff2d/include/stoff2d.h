#pragma once

/* Welcome to the stoff2d engine!
 *
 * stoff2d is a C library for making 2D games.
 *
 * Property of Stoff-Studios pty ltd. You will be sued.
 */

#include <defines.h>         // Types and flags.
#include <settings.h>        // Settings.
#include <clm/clm.h>             // Linear algebra.
#include <animation.h>       // Animation loading and retrieval.
#include <ecs.h>             // Entity Component System.
#include <sprite_renderer.h> // Layered sprite rendering.


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


/******************************** Rendering **********************************/

/* s2d_render_quad
 * ---------------
 * Render a textured quad.
 * For optimal performace, group render calls by texture.
 *
 * To render an entire texture, frame = { 0, 0, 1, 1 }
 */
void s2d_render_quad(
        clmVec2 position,
        clmVec2 size,
        u32     texture,
        Frame   frame);


/* s2d_load_texture
 * ----------------
 * Returns a texture that can be used in s2d_render_quad. 
 * Textures must be png's located in S2D_TEXTURE_FOLDER.
 */
u32 s2d_load_texture(const char* fileName);


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


/********************************* Camera ************************************/

/* s2d_camera_move
 * ---------------
 * Move the camera along the x/y plane.
 */
void s2d_camera_move(f32 dx, f32 dy);

/* s2d_camera_zoom
 * ---------------
 * Zoom the camera by dz.
 */
void s2d_camera_zoom(f32 dz);

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
