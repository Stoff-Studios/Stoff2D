#pragma once

#include <game.h>

typedef enum {
    PARTICLE_TYPE_BULLET,
    PARTICLE_TYPE_BLOOD,
    PARTICLE_TYPE_BIG_BLOOD,
    PARTICLE_TYPE_COUNT
} ParticleType;

void particles_set_game_data_ptr(GameData* gData);

ParticleData* particle_type_data(ParticleType type);

void particle_types_init();
