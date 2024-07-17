#include <particle_types.h>
#include <game.h>

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
        .count         = 4,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 0.5f,
        .velocityRange = (clmVec2) { 10.0f, 50.0f },
        .directionRange = (clmVec2) { 0.0f, 2 * CLM_PI },
        .lowerSize     = 2,
        .upperSize     = 24,
        .birthColour   = (clmVec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "circle_05"
    };

    particleTypes[PARTICLE_TYPE_FIRE] = (s2dParticleType) {
        .count         = 5,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 1.0f,
        .velocityRange = (clmVec2) { 10.0f, 20.0f },
        .directionRange = (clmVec2) { 0.0f, 2 * CLM_PI },
        .lowerSize     = 16,
        .upperSize     = 24,
        .birthColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.2f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "circle_05"
    };

    particleTypes[PARTICLE_TYPE_BLOOD] = (s2dParticleType) {
        .count         = 50,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 0.7f,
        .velocityRange = (clmVec2) { 10.0f, 100.0f },
        .directionRange = (clmVec2) { 0.0f, 2 * CLM_PI },
        .lowerSize     = 8,
        .upperSize     = 24,
        .birthColour   = (clmVec4) { 0.0f, 0.0f, 1.0f, 1.0f },
        .deathColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 0.0f },
        .shader        = gData->canvasShader,
        .spriteName    = "magic_05"
    };

    particleTypes[PARTICLE_TYPE_BIG_BLOOD] = (s2dParticleType) {
        .count         = 1000,
        .lowerLifeTime = 0.2f,
        .upperLifeTime = 1.0f,
        .velocityRange = (clmVec2) { 0.0f, 250.0f },
        .directionRange = (clmVec2) { 0.0f, 2 * CLM_PI },
        .lowerSize     = 8,
        .upperSize     = 24,
        .birthColour   = (clmVec4) { 1.0f, 0.0f, 0.0f, 1.0f },
        .deathColour   = (clmVec4) { 0.5f, 0.0f, 0.0f, 0.5f },
        .shader        = gData->canvasShader,
        .spriteName    = "circle_05"
    };

}
