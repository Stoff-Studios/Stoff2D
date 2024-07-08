#include <particle_types.h>
#include <game.h>
#include <string.h>

static GameData* gData;

static ParticleData particleTypes[PARTICLE_TYPE_COUNT];

const char* circle = "circle_05";

void particles_set_game_data_ptr(GameData* gameData) {
    gData = gameData;
}

ParticleData* particle_type_data(ParticleType type) {
    return &particleTypes[type];
}

void particle_types_init() {
    particleTypes[PARTICLE_TYPE_BULLET] = (ParticleData) {
        .count = 1,
        .lifeTime = 0.3f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -8.0f, -8.0f },
        .upperVelocity = (clmVec2) { 8.0f, 8.0f },
        .lowerSize = (clmVec2) { 16.0f, 16.0f },
        .upperSize = (clmVec2) { 16.0f, 16.0f },
        .birthColour = (clmVec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        .deathColour = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader      = gData->canvasShader
    };
    strncpy(particleTypes[PARTICLE_TYPE_BULLET].spriteName, circle, 32);
    particleTypes[PARTICLE_TYPE_BLOOD] = (ParticleData) {
        .count = 100,
        .lifeTime = 0.15f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -200.0f, -200.0f },
        .upperVelocity = (clmVec2) { 200.0f, 200.0f },
        .lowerSize = (clmVec2) { 4.0f, 4.0f },
        .upperSize = (clmVec2) { 4.0f, 4.0f },
        .birthColour = (clmVec4) { 0.3f, 0.0f, 0.0f, 1.0f },
        .deathColour = (clmVec4) { 0.8f, 0.0f, 0.0f, 0.2f },
        .shader      = gData->canvasShader
    };
    strncpy(particleTypes[PARTICLE_TYPE_BLOOD].spriteName, circle, 32);
    particleTypes[PARTICLE_TYPE_BIG_BLOOD] = (ParticleData) {
        .count = 200,
        .lifeTime = 0.3f,
        .position = (clmVec2) { 0.0f, 0.0f },
        .lowerVelocity = (clmVec2) { -80.0f, -80.0f },
        .upperVelocity = (clmVec2) { 80.0f, 80.0f },
        .lowerSize = (clmVec2) { 8.0f, 8.0f },
        .upperSize = (clmVec2) { 8.0f, 8.0f },
        .birthColour = (clmVec4) { 1.0f, 1.0f, 1.0f, 1.0f },
        .deathColour = (clmVec4) { 1.0f, 1.0f, 1.0f, 0.0f },
        .shader      = gData->canvasShader
    };
    strncpy(particleTypes[PARTICLE_TYPE_BIG_BLOOD].spriteName, circle, 32);
}
