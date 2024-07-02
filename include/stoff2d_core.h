#pragma once

/* Welcome to the stoff2d engine!
 *
 * stoff2d is a C library for making 2D games.
 */

#include <defines.h>         // Types, keycodes and flags.
#include <settings.h>        // Settings.
#include <clm/clm.h>         // Linear algebra.
#include <shader.h>          // Shader loading/usage.
#include <rendertexture.h>   // render buffer helpers.

#ifdef __cplusplus
extern "C" {
#endif

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

/* s2d_get_quad_shader
 * -------------------
 * Returns the default quad shader id used by the engine.
 */
u32 s2d_get_quad_shader();

/* s2d_get_quad_shader
 * -------------------
 * Returns the default text shader used by the engine.
 */
u32 s2d_get_text_shader();

/* s2d_render_quad
 * ---------------
 * Render a textured quad. 
 * For optimal performance, group render calls by texture.
 *
 * position:
 *     bottom-left position of the quad.
 * 
 * size:
 *     width and height (in pixels) of the quad.
 * 
 * colour:
 *     rgba colour of the quad.
 * 
 * texture:
 *     id handle to a texture (obtained by calling s2d_load_texture).
 *     Use S2D_COLOURED_QUAD_TEXTURE if you want to render a coloured quad with
 *     no texture. Although it is better to use s2d_render_coloured_quad in this 
 *     case.
 * 
 * frame:
 *     normalised region of the texture to render. Frame = { 0, 0, 1, 1 } is
 *     the entire texture.
 * 
 * shader:
 *     shader program to use for this quad.
 */
void s2d_render_quad(
        clmVec2 position,
        clmVec2 size,
        clmVec4 colour,
        u32     texture,
        Frame   frame,
        u32     shader);

/* s2d_render_coloured_quad
 * ------------------------
 * Render a coloured quad.
 *
 * position:
 *     bottom-left position of the quad.
 * 
 * size:
 *     width and height (in pixels) of the quad.
 * 
 * colour:
 *     rgba colour of the quad.
 */
void s2d_render_coloured_quad(
        clmVec2 position,
        clmVec2 size,
        clmVec4 colour);

/* s2d_load_texture
 * ----------------
 * Load a texture.
 *
 * fileName:
 *     The name of a png image located in S2D_TEXTURE_FOLDER.
 * 
 * Returns:
 *     texture ID that can be used in s2d_render_quad. 
 */
u32 s2d_load_texture(const char* fileName);

/* s2d_clear_colour
 * ----------------
 * Set the clear colour.
 */
void s2d_clear_colour(clmVec4 colour);

/* s2d_text_render
 * ---------------
 * Render text to the screen.
 * NOTE: s2d_sprite_renderer_render_sprites must be called for any text
 * to show since this uses the sprite renderer.
 *
 * fontName: 
 *     name of a font in the fonts folder, not including .ttf extension
 * position:
 *     screen space position in pixels
 * colour:
 *     colour of the text.
 * layer:
 *     sprite layer
 * formatText:
 *     null terminated format string, format arguments go after this.
 */
void s2d_text_render(
        const char* fontName,
        clmVec2     position,
        clmVec4     colour,
        f32         scale,
        u32         layer,
        const char* formatText,
        ...);

/* s2d_text_render_bitmap
 * ----------------------
 * Debug function to render the bitmap generated from a ttf font loaded from
 * the S2D_FONTS_FOLDER.
 *
 * fontName: 
 *     name of a font in the fonts folder, not including .ttf extension
 * position:
 *     world space position
 * size:
 *     size in pixels to render the bitmap
 * colour:
 *     colour of the text.
 */
void s2d_text_render_bitmap(
        const char* fontName,
        clmVec2     position,
        clmVec2     size,
        clmVec4     colour);

/* s2d_set_texture_slot
 * --------------------
 * Set the corresponding texture slot to texture.
 */
void s2d_set_texture_slot(
        u32 shader,
        const char* uniformName,
        u32 slot,
        u32 texture);

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

/* s2d_set_frame_cap
 * -----------------
 * Change the frame cap.
 *
 * fps:
 *     the desired frames per second, or use macros:
 *     - S2D_FPS_UNCAPPED for uncapped
 *     - S2D_FPS_VSYNC    to enable vsync (undoes previous framecap)
 */
void s2d_set_frame_cap(u32 fps);

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

/* s2d_get_screen_rect()
 * ---------------------
 * Returns the worldspace rect for the currently visible viewport.
 * First two values are bottomleft coordinates, followed by width and height.
 */
clmVec4 s2d_get_screen_rect();

/* s2d_get_viewport_dimensions
 * ---------------------------
 * Returns the current width and height of the viewport.
 * Useful for UI placement.
 */
clmVec2 s2d_get_viewport_dimensions();

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

/* s2d_camera_view
 * ---------------
 * get the current view transform for the camera. Usefull for updating custom
 * shaders.
 */
clmMat4 s2d_camera_view();

/* s2d_camera_projection
 * ---------------------
 * get the current projection transform for the camera. Usefull for updating 
 * custom shaders.
 */
clmMat4 s2d_camera_projection();

/*****************************************************************************/


/******************************* Animation ***********************************/

/* animations_get
 * --------------
 * Retrieve a reference to the animation with key: name.
 * Animations are loaded from res/animations/animations.ani
 * Look at README.ani to learn how to add animations.
 */
Animation* s2d_animations_get(const char* name);

/*****************************************************************************/


/******************************* Particles ***********************************/

/* s2d_particles_add
 * -----------------
 * Spawn a cluster of partcles with properties defined by
 * ParticleData.
 *
 * count determines how many particles are spawned.
 *
 * The velocity and size of each particle will be linearly 
 * distributed between,
 * [lowerVelocity, upperVelocity] and
 * [lowerSize, upperSize] respectively.
 *
 * The colour of each particle will be linearly interpolated
 * between birthColour and deathColour over each particles lifeTime.
 */
void s2d_particles_add(ParticleData* particlesData);

/* s2d_particles_render
 * --------------------
 * Render all particles to the screen.
 */
void s2d_particles_render();

/*****************************************************************************/


/**************************** Sprite Renderer ********************************/

/* Using the sprite renderer allows control over sprite layers. If this is not
 * needed just use s2d_render_quad since it is a little faster.
 *
 * Sprites with a layer of 0 are on top, the higher the layer number the
 * further back the sprite.
 */

/* sprite_renderer_add_sprite
 * --------------------------
 * Add a sprite to the current frame.
 */
void s2d_sprite_renderer_add_sprite(s2dSprite sprite);

/* sprite_renderer_render_sprites
 * ------------------------------
 * Draw all the previously added sprites ordered by layer.
 */
void s2d_sprite_renderer_render_sprites();


/*****************************************************************************/

#ifdef __cplusplus
}
#endif
