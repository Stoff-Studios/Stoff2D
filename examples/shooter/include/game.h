#pragma once

#include <stoff2d.h>

typedef struct {
    bool running;
    bool renderHitboxes;
    bool paused;

    u32  texFont;
    u32  texHitBox;
    u32  texSkeletonWalk;
    u32  texPlayerIdle;

    f32  camSpeed;
    f32  camZoom;
    u32  playerEID;
} GameData;

void game_init();

void game_run();

void game_shutdown();
