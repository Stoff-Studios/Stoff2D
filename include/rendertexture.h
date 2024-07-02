#pragma once

#include <defines.h>

/* s2d_rendertexture_create
 * ------------------------
 * Create a new framebuffer to render to.
 */
u32 s2d_rendertexture_create(
        u32  width,
        u32  height,
        u32  numChannels,
        bool depthBuffer);

/* s2d_rendertexture_set_target
 * ----------------------------
 * Set a texture as a render target.
 */
void s2d_rendertexture_set_target(
        u32 renderTexture,
        u32 textureWidth,
        u32 textureHeight);
