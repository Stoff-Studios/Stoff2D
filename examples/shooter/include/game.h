#pragma once

#include <defines.h>

typedef struct {
    bool running;
    bool renderHitboxes;
    u32  texFont;
    u32  texHitBox;
    f32  camSpeed;
    f32  camZoom;
    u32  playerEID;
} GameData;

void game_init();

void game_run();

void game_shutdown();
