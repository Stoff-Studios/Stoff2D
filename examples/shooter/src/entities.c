#include <stoff2d.h>
#include <entities.h>
#include <particle_types.h>

u32 create_player(clmVec2 pos) {
    u32 eID = s2d_ecs_create_entity();

    Component positionCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_POSITION,
        .position = (PositionComponent) {
            .position = pos
        }
    };

    Component velocityCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_VELOCITY,
        .velocity = (VelocityComponent) {
            .velocity = (clmVec2) { 0.0f, 0.0f },
            .maxSpeed = (clmVec2) { 200.0f, 200.0f}
        }
    };

    Component spriteCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_SPRITE,
        .sprite = (SpriteComponent) {
            .size = (clmVec2) { PLAYER_SIZE, PLAYER_SIZE },
            .colour = (clmVec4) { 0.2f, 0.8f, 0.2f, 1.0f },
            .texture = S2D_COLOURED_QUAD_TEXTURE,
            .frame = (Frame) { 0.0f, 0.0f, 1.0f, 1.0f },
            .layer = 0
        }
    };

    Component playerCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_PLAYER,
        .player = (PlayerComponent) {
            .TODO = 0
        }
    };
 
    s2d_ecs_add_component(positionCmp);
    s2d_ecs_add_component(velocityCmp);
    s2d_ecs_add_component(spriteCmp);
    s2d_ecs_add_component(playerCmp);

    return eID;
}

void create_bullet(clmVec2 position, clmVec2 velocity) {
    u32 eID = s2d_ecs_create_entity();

    Component posCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_POSITION,
        .position = (PositionComponent) {
            .position = position
        }
    };

    Component velCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_VELOCITY,
        .velocity = (VelocityComponent) {
            .velocity = velocity,
            .maxSpeed = (clmVec2) { 0.0f, 0.0f }
        }
    };

    Component spriteCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_SPRITE,
        .sprite = (SpriteComponent) {
            .size = (clmVec2) { 4.0f, 4.0f },
            .colour = (clmVec4) { 0.8f, 0.1f, 0.1f, 1.0f },
            .texture = S2D_COLOURED_QUAD_TEXTURE,
            .frame = (Frame) { 0.0f, 0.0f, 1.0f, 1.0f },
            .layer = 0
        }
    };

    Component deathTimerCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_DEATH_TIMER,
        .deathTimer = (DeathTimerComponent) {
            .timeLeft = 5.0f
        }
    };

    Component particleEmitterCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_PARTICLE_EMITTER,
        .particleEmitter = (ParticleEmitterComponent) {
            .particleType = PARTICLE_TYPE_BULLET,
            .timeUntillNextEmit = 0.0f,
            .emitWaitTime = 0.005f
        }
    };

    s2d_ecs_add_component(posCmp);
    s2d_ecs_add_component(velCmp);
    s2d_ecs_add_component(spriteCmp);
    s2d_ecs_add_component(deathTimerCmp);
    s2d_ecs_add_component(particleEmitterCmp);
}
