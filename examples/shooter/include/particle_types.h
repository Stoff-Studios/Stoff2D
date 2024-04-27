#pragma once

#include <defines.h>

typedef enum {
    PARTICLE_TYPE_BULLET,
    PARTICLE_TYPE_COUNT
} ParticleType;

ParticleData* particle_type_data(ParticleType type);

void particle_types_init();
