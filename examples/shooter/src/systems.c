#include <stoff2d.h>
#include <stoff2d_ecs.h>
#include <entities.h>
#include <particle_types.h>

void system_render() {
    s2dComponentMap* spriteCmps = s2d_ecs_get_bucket(CMP_TYPE_SPRITE);
    for (u64 i = 0; i < s2d_component_map_tablesize(spriteCmps); i++) {
        Component* spriteCmp = s2d_component_map_at(spriteCmps, i);
        u32 eID = spriteCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }
        Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        s2d_sprite_renderer_add_sprite(
                (s2dSprite) { 
                    .position = posCmp->position.position,
                    .size = spriteCmp->sprite.size,
                    .colour = spriteCmp->sprite.colour,
                    .texture = spriteCmp->sprite.texture,
                    .frame = spriteCmp->sprite.frame,
                    .layer = spriteCmp->sprite.layer
                    }
                );
    }
    s2d_sprite_renderer_render_sprites();
}

void system_move(f32 timeStep) {
    s2dComponentMap* velCmps = s2d_ecs_get_bucket(CMP_TYPE_VELOCITY);
    for (u64 i = 0; i < s2d_component_map_tablesize(velCmps); i++) {
        Component* velCmp = s2d_component_map_at(velCmps, i);
        u32 eID = velCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }
        Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        posCmp->position.position.x += velCmp->velocity.velocity.x * timeStep;
        posCmp->position.position.y += velCmp->velocity.velocity.y * timeStep;
    }
}

void system_control(f32 timeStep) {
    s2dComponentMap* playerCmps = s2d_ecs_get_bucket(CMP_TYPE_PLAYER);
    for (u64 i = 0; i < s2d_component_map_tablesize(playerCmps); i++) {
        Component* controlCmp = s2d_component_map_at(playerCmps, i);
        u32 eID = controlCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_VELOCITY)) {
            continue;
        }
        Component* cmp = s2d_ecs_get_component(eID, CMP_TYPE_VELOCITY);
        VelocityComponent* velCmp = &cmp->velocity;
        velCmp->velocity = (clmVec2) { 0.0f, 0.0f };
        if (s2d_keydown(S2D_KEY_W)) {
            velCmp->velocity.y += velCmp->maxSpeed.y;
        }
        if (s2d_keydown(S2D_KEY_A)) {
            velCmp->velocity.x -= velCmp->maxSpeed.x;
        }
        if (s2d_keydown(S2D_KEY_S)) {
            velCmp->velocity.y -= velCmp->maxSpeed.y;
        }
        if (s2d_keydown(S2D_KEY_D)) {
            velCmp->velocity.x += velCmp->maxSpeed.x;
        }

        f32 bulletSpeed = 600.0f;
        cmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        PositionComponent* posCmp = &cmp->position;
        clmVec2 bulletPosition = (clmVec2) {
            posCmp->position.x + 6.0f,
            posCmp->position.y + 6.0f
        };
        clmVec2 vel = velCmp->velocity;

        static f32 canShoot = 0.0f;
        f32 shootTime = 0.2f;
        canShoot -= timeStep;
        if (canShoot <= 0) {
            canShoot = 0.0f;
            if (s2d_keydown(S2D_KEY_LEFT)) {
                canShoot = shootTime;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x - bulletSpeed, vel.y });
            } else if (s2d_keydown(S2D_KEY_RIGHT)) {
                canShoot = shootTime;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x + bulletSpeed, vel.y });
            } else if (s2d_keydown(S2D_KEY_UP)) {
                canShoot = shootTime;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x, vel.y + bulletSpeed });
            } else if (s2d_keydown(S2D_KEY_DOWN)) {
                canShoot = shootTime;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x, vel.y - bulletSpeed });
            }
        }
    }
}

void system_death_timer(f32 timeStep) {
    s2dComponentMap* timers = s2d_ecs_get_bucket(CMP_TYPE_DEATH_TIMER);
    for (u64 i = 0; i < s2d_component_map_tablesize(timers); i++) {
        Component* timer = s2d_component_map_at(timers, i);
        u32 eID = timer->eID;
        if (eID == NO_ENTITY) {
            continue;
        }
        timer->deathTimer.timeLeft -= timeStep;
        if (timer->deathTimer.timeLeft <= 0) {
            s2d_ecs_delete_entity(eID);
        }
    }
}

void system_particles(f32 timeStep) {
    s2dComponentMap* emitters = s2d_ecs_get_bucket(CMP_TYPE_PARTICLE_EMITTER);
    for (u64 i = 0; i < s2d_component_map_tablesize(emitters); i++) {
        Component* emitter = s2d_component_map_at(emitters, i);
        emitter->particleEmitter.timeUntillNextEmit -= timeStep;
        if (emitter->particleEmitter.timeUntillNextEmit <= 0.0f) {
            emitter->particleEmitter.timeUntillNextEmit = 
                emitter->particleEmitter.emitWaitTime;
            u32 eID = emitter->eID;
            if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
                continue;
            }
            Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
            ParticleData pData;
            pData = *particle_type_data(emitter->particleEmitter.particleType);
            pData.position = posCmp->position.position;
            s2d_particles_add(&pData);
        }
    }
}


