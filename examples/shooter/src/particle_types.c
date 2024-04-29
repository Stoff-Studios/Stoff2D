#include <particle_types.h>

static ParticleData particleTypes[PARTICLE_TYPE_COUNT];

ParticleData* particle_type_data(ParticleType type) {
    return &particleTypes[type];
}

void particle_types_init() {
    particleTypes[PARTICLE_TYPE_BULLET] = (ParticleData) {
        .count = 2,
        .lifeTime = 0.2f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -30.0f, -30.0f },
        .upperVelocity = (clmVec2) { 30.0f, 30.0f },
        .lowerSize = (clmVec2) { 2.0f, 2.0f },
        .upperSize = (clmVec2) { 5.0f, 5.0f },
        .birthColour = (clmVec4) { 0.2f, 0.2f, 0.2f, 1.0f },
        .deathColour = (clmVec4) { 0.8f, 0.8f, 0.8f, 0.0f }
    };
    particleTypes[PARTICLE_TYPE_BLOOD] = (ParticleData) {
        .count = 100,
        .lifeTime = 0.2f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -200.0f, -200.0f },
        .upperVelocity = (clmVec2) { 200.0f, 200.0f },
        .lowerSize = (clmVec2) { 0.1f, 0.1f },
        .upperSize = (clmVec2) { 3.0f, 3.0f },
        .birthColour = (clmVec4) { 0.8f, 0.2f, 0.2f, 1.0f },
        .deathColour = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f }
    };
    particleTypes[PARTICLE_TYPE_BIG_BLOOD] = (ParticleData) {
        .count = 200,
        .lifeTime = 0.3f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -100.0f, -100.0f },
        .upperVelocity = (clmVec2) { 100.0f, 100.0f },
        .lowerSize = (clmVec2) { 0.4f, 0.4f },
        .upperSize = (clmVec2) { 5.0f, 5.0f },
        .birthColour = (clmVec4) { 0.8f, 0.2f, 0.2f, 1.0f },
        .deathColour = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f }
    };
}
