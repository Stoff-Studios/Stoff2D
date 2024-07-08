#pragma once

#include <stoff2d_core.h>

typedef struct {
    bool running;
    bool renderHitboxes;
    bool paused;

    RenderTexture canvas;       // rendertexture
    u32           canvasShader; // for rendering to canvas texture.
    u32           screenShader; // for rendering canvas to screen.

    u32  texFont;
    u32  texHitBox;
    u32  texSkeletonWalk;
    u32  texSkeletonDie;
    u32  texPlayerIdle;

    f32  camSpeed;
    f32  camZoom;
    u32  playerEID;

    f32  shotTimer;
} GameData;

void game_init();

void game_run();

void game_shutdown();
