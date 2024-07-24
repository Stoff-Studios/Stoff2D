#pragma once

#include <defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/* s2d_rendertexture_create
 * ------------------------
 * Create a new framebuffer to render to.
 */
s2dRenderTexture s2d_rendertexture_create(
        u32  width,
        u32  height,
        u32  numChannels,
        bool depthBuffer);

/* s2d_rendertexture_set_target
 * ----------------------------
 * Set a texture as a render target.
 */
void s2d_rendertexture_set_target(s2dRenderTexture renderTexture);

/* s2d_rendertexture_set_target_screen
 * -----------------------------------
 * Set the screen to be the render target.
 */
void s2d_rendertexture_set_target_screen();

#ifdef __cplusplus
}
#endif
