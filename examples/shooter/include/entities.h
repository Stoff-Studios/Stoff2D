#pragma once

#include <game.h>

// Misc
#define CLEAR_COLOUR ((clmVec4) { 0.6f, 0.6f, 0.65f, 1.0f })


// Player
#define PLAYER_SIZE  ((clmVec2) { 32, 48 })
#define PLAYER_SPEED 100.0f
#define PLAYER_HP 50.0f
#define PLAYER_INVINSIBILITY_TIME 0.5f

// Bullet
#define BULLET_SPEED 250.0f

// Enemy
#define ENEMY_SIZE               ((clmVec2) { 22, 33 })
#define ENEMY_SPEED              40.0f
#define ENEMY_HP                 40.0f
#define ENEMY_INVINSIBILITY_TIME 0.1f

// Layers
#define TEXT_LAYER   0
#define HITBOX_LAYER 1
#define ENEMY_LAYER  2
#define PLAYER_LAYER 3

void entites_set_game_data_ptr(GameData* gData);

u32  create_player(clmVec2 position);
void create_bullet(clmVec2 position, clmVec2 velocity);
void create_enemy(clmVec2 position);
void create_skeleton_death_animation(clmVec2 pos);
