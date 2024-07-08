#pragma once

#include <game.h>

void systems_set_game_data_ptr(GameData* gData);

void system_render(f32 timeStep);
void system_move(f32 timeStep);
void system_control(f32 timeStep);
void system_death_timer(f32 timeStep);
void system_particles(f32 timeStep);
void system_enemy(f32 timeStep);
void system_damage_cooldown(f32 timeStep);
void system_damage();
void system_invinsibility(f32 timeStep);
void system_move_hitboxes();
void system_spawn_enemies(f32 timeStep);
void system_animation(f32 timeStep);
void system_fps(f32 timeStep);
