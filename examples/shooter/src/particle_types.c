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
        .lowerVelocity = (clmVec2) { -20.0f, -20.0f },
        .upperVelocity = (clmVec2) { 20.0f, 20.0f },
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
        .lowerVelocity = (clmVec2) { -10.0f, -2.0f },
        .upperVelocity = (clmVec2) { 40.0f, 20.0f },
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
