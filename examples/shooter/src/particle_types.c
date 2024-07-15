#include <particle_types.h>
#include <game.h>
#include <string.h>

const char* circle5 = "circle_05";

static GameData* gData;

static s2dParticleType particleTypes[PARTICLE_TYPE_COUNT];

void particles_set_game_data_ptr(GameData* gameData) {
    gData = gameData;
}

const s2dParticleType* particle_type_data(ParticleType type) {
    return &particleTypes[type];
}

void particle_types_init() {

    particleTypes[PARTICLE_TYPE_BULLET] = (s2dParticleType) {
        .count         = 5,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 0.7f,
        .lowerVelocity = (clmVec2) { -20.0f, -20.0f },
        .upperVelocity = (clmVec2) { 20.0f, 20.0f },
        .lowerSize     = 4,
        .upperSize     = 16,
        .birthColour   = (clmVec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "magic_05"
    };

    particleTypes[PARTICLE_TYPE_BLOOD] = (s2dParticleType) {
        .count         = 50,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 0.7f,
        .lowerVelocity = (clmVec2) { -80.0f, -80.0f },
        .upperVelocity = (clmVec2) { 80.0f, 80.0f },
        .lowerSize     = 4,
        .upperSize     = 16,
        .birthColour   = (clmVec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "magic_05"
    };

    particleTypes[PARTICLE_TYPE_BIG_BLOOD] = (s2dParticleType) {
        .count         = 150,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 0.8f,
        .lowerVelocity = (clmVec2) { -50.0f, -50.0f },
        .upperVelocity = (clmVec2) { 50.0f, 50.0f },
        .lowerSize     = 2,
        .upperSize     = 24,
        .birthColour   = (clmVec4) { 1.0f, 1.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 1.0f, 1.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "circle_05"
    };

}
