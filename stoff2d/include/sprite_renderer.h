#pragma once

#include <defines.h>
#include <component.h>

/* The purpose of this is to order sprites by layer so we can render them in
 * the correct order without having to worry about it.
 *
 * For example sprites with a layer of 0 will always be rendered before anything
 * else so we can just set a layer and chill.
 */

/* sprite_renderer_add_sprite
 * --------------------------
 * Add a sprite to the current frame.
 */
void s2d_sprite_renderer_add_sprite(SpriteComponent sprite, clmVec2 position);

/* sprite_renderer_render_sprites
 * ------------------------------
 * Draw all the previously added sprites ordered by layer.
 */
void s2d_sprite_renderer_render_sprites();

/* sprite_renderer_init
 * --------------------
 * Initialise the sprite renderer.
 * NOTE: this is called by the engine startup, no need to call.
 */
void sprite_renderer_init();

/* sprite_renderer_shutdown
 * ------------------------
 * free memory used by the sprite renderer.
 * NOTE: this is called by the engine shutdown, no need to call.
 */
void sprite_renderer_shutdown();
