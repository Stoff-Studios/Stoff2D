#include <rendertexture.h>
#include <glad/glad.h>
#include <stdio.h>


RenderTexture s2d_rendertexture_create(
        u32  textureWidth,
        u32  textureHeight,
        u32  numChannels,
        bool depthBuffer) {

    // create framebuffer.
    u32 frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    // create texture.
    u32 renderTexture;
    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA, // TODO: configurable
            textureWidth,
            textureHeight,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);

    // texture params.
    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER, 
            GL_LINEAR);
    glTexParameteri(
            GL_TEXTURE_2D, 
            GL_TEXTURE_MAG_FILTER, 
            GL_NEAREST);
    glTexParameteri(
            GL_TEXTURE_2D, 
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE);
    glTexParameteri(
            GL_TEXTURE_2D, 
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE);

    // optional depth buffer.
    if (depthBuffer) {
        u32 dBuffer;
        glGenRenderbuffers(1, & dBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, dBuffer);
        glRenderbufferStorage(
                GL_RENDERBUFFER,
                GL_DEPTH_COMPONENT,
                textureWidth, 
                textureHeight);
        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER,
                dBuffer);
    }

    // attach texture to framebuffer.
    glFramebufferTexture(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            renderTexture,
            0);
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "[S2D Error] failed to create font render texture\n");
    }

    return (RenderTexture) {
        .frameBufferID = frameBuffer,
        .textureID     = renderTexture,
        .width         = textureWidth,
        .height        = textureHeight
    };
}

void s2d_rendertexture_set_target(RenderTexture renderTexture) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderTexture.frameBufferID);
    glViewport(0, 0, renderTexture.width, renderTexture.height);
}
