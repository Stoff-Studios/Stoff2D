#pragma once

#include <defines.h>

// Player
#define PLAYER_SPEED 200.0f
#define PLAYER_SIZE  16.0f

// Bullet
#define BULLET_SPEED 600.0f

u32  create_player(clmVec2 position);
void create_bullet(clmVec2 position, clmVec2 velocity);
