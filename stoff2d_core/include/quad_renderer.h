#pragma once

#include <defines.h>

typedef struct _QuadRenderer* QuadRenderer;

/* quad_renderer_create
 * --------------------
 * create a new quadrenderer
 */
QuadRenderer quad_renderer_create();

/* quad_renderer_shutdown
 * ----------------------
 * free resources used by quadrenderer
 */
void quad_renderer_shutdown(QuadRenderer renderer);

/* quad_renderer_flush
 * -------------------
 * render the current batch to the render target
 */
void quad_renderer_flush(QuadRenderer renderer);

/* quad_renderer_submit_quad
 * -------------------------
 * add a quad into the batch
 */
void quad_renderer_submit_quad(
        QuadRenderer renderer,
        clmVec2      position, 
        clmVec2      size, 
        clmVec4      colour,
        u32          texID,  
        s2dFrame     texSubRegion,
        u32          shader);

/* quad_renderer_set_texture_slot
 * ------------------------------
 * binds texIDs to a texture slot in order from 1 - count, binding persists
 * until this is called again or quad_renderer_reset_texture_slots is called
 */
void quad_renderer_set_texture_slots(
        QuadRenderer renderer,
        u32 count,
        u32 texIDs[]);

/* quad_renderer_reset_texture_slot
 * --------------------------------
 * unbind texture slots
 */
void quad_renderer_reset_texture_slots(QuadRenderer renderer);

/* quad_renderer_print_stats
 * -------------------------
 * print renderer statistics
 */
void quad_renderer_print_stats(QuadRenderer renderer);

/* quad_renderer_print_stats
 * -------------------------
 * reset stats for a new frame
 */
void quad_renderer_reset_stats(QuadRenderer renderer);
