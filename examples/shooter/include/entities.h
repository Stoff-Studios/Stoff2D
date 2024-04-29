#pragma once

#include <game.h>

// Player
#define PLAYER_SPEED 200.0f
#define PLAYER_SIZE  16.0f

// Bullet
#define BULLET_SPEED 600.0f

// Enemy
#define ENEMY_SPEED              50.0f
#define ENEMY_HP                 50.0f
#define ENEMY_INVINSIBILITY_TIME 0.2f

// Layers
#define HITBOX_LAYER 0
#define ENEMY_LAYER  1
#define PLAYER_LAYER 2

void entites_set_game_data_ptr(GameData* gData);

u32  create_player(clmVec2 position);
void create_bullet(clmVec2 position, clmVec2 velocity);
void create_enemy(clmVec2 position);
