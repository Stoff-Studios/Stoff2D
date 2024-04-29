#include <stoff2d.h>
#include <stoff2d_ecs.h>
#include <entities.h>
#include <particle_types.h>

static GameData* gData;

void entites_set_game_data_ptr(GameData* gameData) {
    gData = gameData;
}

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
            .layer = PLAYER_LAYER
        }
    };

    Component playerCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_PLAYER,
        .player = (PlayerComponent) {
            .TODO = 0
        }
    };

    Component hitboxCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_HITBOX,
        .hitbox = (HitBoxComponent) {
            .position = (clmVec2) { 0.0f, 0.0f },
            .size = (clmVec2) { PLAYER_SIZE, PLAYER_SIZE }
        }
    };
 
    s2d_ecs_add_component(positionCmp);
    s2d_ecs_add_component(velocityCmp);
    s2d_ecs_add_component(spriteCmp);
    s2d_ecs_add_component(playerCmp);
    s2d_ecs_add_component(hitboxCmp);

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
            .layer = PLAYER_LAYER
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

    Component hitboxCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_HITBOX,
        .hitbox = (HitBoxComponent) {
            .position = (clmVec2) { 0.0f, 0.0f },
            .size = (clmVec2) { 4.0f, 4.0f }
        }
    };

    Component damageCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_DAMAGE,
        .damage = (DamageComponent) {
            .damage = 10.0f,
            .cooldown = 0.1f,
            .currentCooldown = 0.0f,
            .deleteOnHit = true
        }
    };

    s2d_ecs_add_component(posCmp);
    s2d_ecs_add_component(velCmp);
    s2d_ecs_add_component(spriteCmp);
    s2d_ecs_add_component(deathTimerCmp);
    s2d_ecs_add_component(particleEmitterCmp);
    s2d_ecs_add_component(hitboxCmp);
    s2d_ecs_add_component(damageCmp);
}

void create_enemy(clmVec2 position) {
    u32 eID = s2d_ecs_create_entity();

    Component positionCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_POSITION,
        .position = (PositionComponent) {
            .position = position
        }
    };

    Component velocityCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_VELOCITY,
        .velocity = (VelocityComponent) {
            .velocity = (clmVec2) { 0.0f, 0.0f },
            .maxSpeed = (clmVec2) { 150.0f, 150.0f}
        }
    };

    Component spriteCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_SPRITE,
        .sprite = (SpriteComponent) {
            .size = (clmVec2) { PLAYER_SIZE, PLAYER_SIZE },
            .colour = (clmVec4) { 0.2f, 0.0f, 0.0f, 1.0f },
            .texture = S2D_COLOURED_QUAD_TEXTURE,
            .frame = (Frame) { 0.0f, 0.0f, 1.0f, 1.0f },
            .layer = ENEMY_LAYER
        }
    };

    Component enemyCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_ENEMY,
        .enemy = (EnemeyComponent) {
            .playerEID = NO_ENTITY
        }
    };

    Component hitboxCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_HITBOX,
        .hitbox = (HitBoxComponent) {
            .position = (clmVec2) { 0.0f, 0.0f },
            .size = (clmVec2) { PLAYER_SIZE, PLAYER_SIZE }
        }
    };

    Component healthCmp = (Component) {
        .eID = eID,
        .type = CMP_TYPE_HEALTH,
        .health = (HealthComponent) {
            .hp = ENEMY_HP,
            .maxHp = ENEMY_HP,
            .invinsibilityTime = ENEMY_INVINSIBILITY_TIME,
            .invinsibilityTimer = 0.0f
        }
    };
 
    s2d_ecs_add_component(positionCmp);
    s2d_ecs_add_component(velocityCmp);
    s2d_ecs_add_component(spriteCmp);
    s2d_ecs_add_component(enemyCmp);
    s2d_ecs_add_component(hitboxCmp);
    s2d_ecs_add_component(healthCmp);

}
