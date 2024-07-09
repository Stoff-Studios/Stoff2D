#include <stoff2d_core.h>
#include <stoff2d_ecs.h>
#include <entities.h>
#include <particle_types.h>
#include <stdlib.h>
#include <math.h>

static GameData* gData;

void systems_set_game_data_ptr(GameData* gameData) {
    gData = gameData;
}

f32 randfnorm() {
    return ((f32) rand() / (f32) RAND_MAX);
}

clmVec2 get_center(clmVec2 pos, clmVec2 size) {
    return clm_v2_add(pos, clm_v2_scalar_mul(0.5f, size));
}

bool hitboxes_collided(HitBoxComponent a, HitBoxComponent b) {
    f32 aLeft   = a.position.x;
    f32 aRight  = a.position.x + a.size.x;
    f32 aBottom = a.position.y;
    f32 aTop    = a.position.y + a.size.y;

    f32 bLeft   = b.position.x;
    f32 bRight  = b.position.x + b.size.x;
    f32 bBottom = b.position.y;
    f32 bTop    = b.position.y + b.size.y;

    return !(aLeft > bRight || 
            aRight < bLeft ||
            aTop < bBottom ||
            aBottom > bTop);
}

void system_render(f32 timeStep) {
    // render to the canvas.
    s2d_set_blend_mode(BLEND_MODE_RENDER_TO_TEXTURE);
    s2d_rendertexture_set_target(gData->canvas);
    s2d_clear_colour(CLEAR_COLOUR);
    s2d_clear();

    s2dComponentMap* spriteCmps = s2d_ecs_get_bucket(CMP_TYPE_SPRITE);
    for (u64 i = 0; i < s2d_component_map_tablesize(spriteCmps); i++) {
        Component* spriteCmp = s2d_component_map_at(spriteCmps, i);
        u32 eID = spriteCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }

        Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        clmVec4 colour = spriteCmp->sprite.colour;
        // visual indicator of invinsibility.
        if (s2d_ecs_entity_has(eID, CMP_TYPE_HEALTH)) {
            Component* hpCmp = s2d_ecs_get_component(eID, CMP_TYPE_HEALTH);
            if (hpCmp->health.invinsibilityTimer > 0.0f) {
                colour = (clmVec4) { 
                    colour.r * 3.0f, 
                    colour.g * 3.0f,
                    colour.b * 3.0f,
                    colour.a
                };
            }
        }

        s2d_sprite_renderer_add_sprite(
                (s2dSprite) { 
                    .position = posCmp->position.position,
                    .size = spriteCmp->sprite.size,
                    .colour = colour,
                    .texture = spriteCmp->sprite.texture,
                    .frame = spriteCmp->sprite.frame,
                    .layer = spriteCmp->sprite.layer,
                    .shader = gData->canvasShader
                });
    }

    if (gData->renderHitboxes) {
        s2dComponentMap* hitboxes = s2d_ecs_get_bucket(CMP_TYPE_HITBOX);
        for (u64 i = 0; i < s2d_component_map_tablesize(hitboxes); i++) {
            Component* hitboxCmp = s2d_component_map_at(hitboxes, i);
            if (hitboxCmp->eID == NO_ENTITY) {
                continue;
            }
            s2d_sprite_renderer_add_sprite(
                    (s2dSprite) {
                        .position = hitboxCmp->hitbox.position,
                        .size = hitboxCmp->hitbox.size,
                        .colour = (clmVec4) { 1.0f, 0.0f, 0.0f, 1.0f },
                        .texture = gData->texHitBox,
                        .frame = (s2dFrame) { 0.0f, 0.0f, 1.0f, 1.0f },
                        .layer = HITBOX_LAYER,
                        .shader = gData->canvasShader
                    });
        }
    }

    // flush all rendering to the canvas
    s2d_sprite_renderer_render_sprites();
    s2d_particles_render();
    s2d_render_flush();

    // switch target to the screen and draw our canvas to it.
    s2d_set_blend_mode(BLEND_MODE_RENDER_TEXTURE_TO_SCREEN);
    s2d_rendertexture_set_target_screen();
    s2d_clear_colour((clmVec4) { 0.0f, 0.0f, 0.0f, 0.0f });
    s2d_clear();
    s2d_render_quad(
            clm_v2_scalar_mul(0.125f, s2d_get_viewport_dimensions()),
            clm_v2_scalar_mul(0.75f, s2d_get_viewport_dimensions()),
            (clmVec4) { 1.0f, 1.0f, 1.0f, 1.0 },
            gData->canvas.textureID,
            S2D_ENTIRE_TEXTURE,
            gData->screenShader);
    s2d_render_flush();

    s2d_set_blend_mode(BLEND_MODE_RENDER_TO_SCREEN);

    // render top banner
    static f32 x = 0.0f;
    x += 2 * timeStep;
    s2d_text_render(
            "zerovelo",            
            (clmVec2) { 
                .x = 20.0f,
                .y = s2d_get_viewport_dimensions().y - 100
            },
            (clmVec4) {            
                fabs(sinf(x)),    
                fabs(cosf(2 * x)), 
                fabs(sinf(3*x)), 
                1.0f 
            },
            2.0f,
            TEXT_LAYER,
            "%s", 
            "Stoff2D"
            );


    // render fps
    static i32 frames = 0;
    static i32 displayFrames = 0;
    static f32 time = 0.0f;

    frames++;
    time += timeStep;

    if (time >= 0.25f) {
        displayFrames = frames;
        frames = 0;
        time   = 0.0f;
    }

    s2d_text_render(
            "BigBlueTerm",
            (clmVec2) { 10.0f, 12.0f },
            (clmVec4) {            
                fabs(sinf(x)),    
                fabs(cosf(2 * x)), 
                fabs(sinf(3*x)), 
                1.0f 
            },
            0.5f,
            TEXT_LAYER,
            "fps: %d",
            4 * displayFrames
            );

    // render kill count
    s2d_text_render(
            "BigBlueTerm",
            clm_v2_scalar_mul(0.128f, s2d_get_viewport_dimensions()),
            (clmVec4) {            
                fabs(sinf(x)),    
                fabs(cosf(2 * x)), 
                fabs(sinf(3*x)), 
                1.0f 
            },
            1.0f,
            TEXT_LAYER,
            "foes foresaken: %u",
            gData->killCount);

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

        cmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        PositionComponent* posCmp = &cmp->position;

        // Lock camera to player.
        if (s2d_ecs_entity_has(eID, CMP_TYPE_SPRITE)) {
            Component* spriteCmp = s2d_ecs_get_component(eID, CMP_TYPE_SPRITE);
            s2d_camera_set_pos(get_center(
                        posCmp->position,
                        spriteCmp->sprite.size));
        }

        clmVec2 bulletPosition = clm_v2_add(posCmp->position,
                clm_v2_scalar_mul(0.5f, PLAYER_SIZE));
        clmVec2 vel = velCmp->velocity;
        f32 bulletSpeed = BULLET_SPEED;
        const f32 shotRate = 4.0f;
        gData->shotTimer += timeStep;
        if (gData->shotTimer >= 1.0f / shotRate) {
            gData->shotTimer = 100.0f; // cap this so doesnt go negative.
            if (s2d_keydown(S2D_KEY_LEFT)) {
                gData->shotTimer = 0.0f;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { -bulletSpeed, vel.y });
            } else if (s2d_keydown(S2D_KEY_RIGHT)) {
                gData->shotTimer = 0.0f;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { bulletSpeed, vel.y });
            } else if (s2d_keydown(S2D_KEY_UP)) {
                gData->shotTimer = 0.0f;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x, bulletSpeed });
            } else if (s2d_keydown(S2D_KEY_DOWN)) {
                gData->shotTimer = 0.0f;
                create_bullet(
                        bulletPosition, 
                        (clmVec2) { vel.x, -bulletSpeed });
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
            s2d_particles_add(
                    particle_type_data(emitter->particleEmitter.particleType),
                    posCmp->position.position);
        }
    }
}

void system_enemy(f32 timeStep) {
    if (!s2d_ecs_entity_has(gData->playerEID, CMP_TYPE_POSITION)) {
        return;
    }
    s2dComponentMap* enemies = s2d_ecs_get_bucket(CMP_TYPE_ENEMY);
    for (u64 i = 0; i < s2d_component_map_tablesize(enemies); i++) {
        Component* enemy = s2d_component_map_at(enemies, i);
        u32 eID = enemy->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_VELOCITY) ||
                !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }
        Component* playerPosCmp = s2d_ecs_get_component(
                gData->playerEID, CMP_TYPE_POSITION);
        Component* enemyPosCmp = s2d_ecs_get_component(
                eID, CMP_TYPE_POSITION);
        Component* enemyVelCmp = s2d_ecs_get_component(
                eID, CMP_TYPE_VELOCITY);
        clmVec2 playerPos = clm_v2_add(playerPosCmp->position.position,
                clm_v2_scalar_mul(0.5f, PLAYER_SIZE));
        clmVec2 enemyPos = clm_v2_add(enemyPosCmp->position.position,
                clm_v2_scalar_mul(0.5f, ENEMY_SIZE));
        if (enemyPos.x > playerPos.x) {
            enemyVelCmp->velocity.velocity.x = -ENEMY_SPEED;
        } else if (enemyPos.x < playerPos.x) {
            enemyVelCmp->velocity.velocity.x = ENEMY_SPEED;
        } else {
            enemyVelCmp->velocity.velocity.x = 0.0f;
        }
        if (enemyPos.y > playerPos.y) {
            enemyVelCmp->velocity.velocity.y = -ENEMY_SPEED;
        } else if (enemyPos.y < playerPos.y) {
            enemyVelCmp->velocity.velocity.y = ENEMY_SPEED;
        } else {
            enemyVelCmp->velocity.velocity.y = 0.0f;
        }
    }
}

void system_damage_cooldown(f32 timeStep) {
    s2dComponentMap* damages = s2d_ecs_get_bucket(CMP_TYPE_DAMAGE);
    for (u64 i = 0; i < s2d_component_map_tablesize(damages); i++) {
        Component* damageCmp = s2d_component_map_at(damages, i);
        u32 eID = damageCmp->eID;
        if (eID == NO_ENTITY) {
            continue;
        }
        damageCmp->damage.currentCooldown -= timeStep;
        if (damageCmp->damage.currentCooldown <= 0.0f) {
            damageCmp->damage.currentCooldown = 0.0f;
        }
    }
}

void system_move_hitboxes() {
    s2dComponentMap* hitboxes = s2d_ecs_get_bucket(CMP_TYPE_HITBOX);
    for (u64 i = 0; i < s2d_component_map_tablesize(hitboxes); i++) {
        Component* hitboxCmp = s2d_component_map_at(hitboxes, i);
        u32 hitboxEID = hitboxCmp->eID;
        if (hitboxEID == NO_ENTITY ||
                !s2d_ecs_entity_has(hitboxEID, CMP_TYPE_POSITION)) {
            continue;
        }
        hitboxCmp->hitbox.position = 
            s2d_ecs_get_component(
                    hitboxEID, 
                    CMP_TYPE_POSITION)->position.position;
    }
}

void system_invinsibility(f32 timeStep) {
    s2dComponentMap* healths = s2d_ecs_get_bucket(CMP_TYPE_HEALTH);
    for (u64 i = 0; i < s2d_component_map_tablesize(healths); i++) {
        Component* healthCmp = s2d_component_map_at(healths, i);
        u32 healthEID = healthCmp->eID;
        if (healthCmp == NO_ENTITY) {
            continue;
        }
        healthCmp->health.invinsibilityTimer -= timeStep;
        if (healthCmp->health.invinsibilityTimer <= 0.0f) {
            healthCmp->health.invinsibilityTimer = 0.0f;
        }
    }
}

void system_damage() {
    s2dComponentMap* healths = s2d_ecs_get_bucket(CMP_TYPE_HEALTH);
    for (u64 i = 0; i < s2d_component_map_tablesize(healths); i++) {
        Component* healthCmp = s2d_component_map_at(healths, i);
        u32 healthEID = healthCmp->eID;
        if (healthCmp == NO_ENTITY ||
                !s2d_ecs_entity_has(healthEID, CMP_TYPE_HITBOX) ||
                healthCmp->health.invinsibilityTimer > 0.0f) {
            continue;
        }
        Component* healthHB = s2d_ecs_get_component(healthEID, CMP_TYPE_HITBOX);
        s2dComponentMap* damagers = s2d_ecs_get_bucket(CMP_TYPE_DAMAGE);
        for (u64 i = 0; i < s2d_component_map_tablesize(damagers); i++) {
            Component* damageCmp = s2d_component_map_at(damagers, i);
            u32 damageEID = damageCmp->eID;
            if (damageEID == NO_ENTITY || 
                    !s2d_ecs_entity_has(damageEID, CMP_TYPE_HITBOX) ||
                    damageCmp->damage.currentCooldown > 0.0f) {
                continue;
            }

            Component* damageHB = s2d_ecs_get_component(damageEID, CMP_TYPE_HITBOX);
            if (hitboxes_collided(healthHB->hitbox, damageHB->hitbox)) {
                clmVec2 pPos = clm_v2_add(
                        damageHB->hitbox.position,
                        clm_v2_scalar_mul(0.5f, damageHB->hitbox.size));

                s2d_particles_add(
                        particle_type_data(PARTICLE_TYPE_BLOOD),
                        pPos);

                healthCmp->health.hp -= damageCmp->damage.damage;

                if (healthCmp->health.hp <= 0.0f) {
                    pPos = clm_v2_add(
                            healthHB->hitbox.position,
                            clm_v2_scalar_mul(0.5f, healthHB->hitbox.size));

                    s2d_particles_add(
                            particle_type_data(PARTICLE_TYPE_BIG_BLOOD),
                            pPos);

                    create_skeleton_death_animation(healthHB->hitbox.position);

                    s2d_ecs_delete_entity(healthEID);

                    gData->killCount++;
                } else {
                    healthCmp->health.invinsibilityTimer = 
                        healthCmp->health.invinsibilityTime;
                }
                if (damageCmp->damage.deleteOnHit) {
                    s2d_ecs_delete_entity(damageEID);
                }
                break;
            }
        }
    }
}

void system_spawn_enemies(f32 timeStep) {
    clmVec2 lower = { -200.0f, -200.0f };
    clmVec2 diff = { 400.0f, 400.0f };
    static f32 spawnTimer = 0.0f;
    spawnTimer -= timeStep;
    if (spawnTimer <= 0.0f) {
        spawnTimer = 2.0f; // one per second.
        create_enemy((clmVec2) { 
                lower.x + (diff.x * randfnorm()),
                lower.y + (diff.y * randfnorm())
                });
    }
}

void system_animation(f32 timeStep) {
    s2dComponentMap* animations = s2d_ecs_get_bucket(CMP_TYPE_ANIMATION);
    for (u64 i = 0; i < s2d_component_map_tablesize(animations); i++) {
        Component* animationCmp = s2d_component_map_at(animations, i);
        u32 aniEID = animationCmp->eID;
        if (aniEID == NO_ENTITY) {
            continue;
        }
        // increment all animation index.
        animationCmp->animation.aniIndex += animationCmp->animation.aniSpeed * timeStep;
        if (animationCmp->animation.aniIndex >= 
                animationCmp->animation.animation->frameCount) {
            animationCmp->animation.aniIndex = 0.0f;
        }
        // update the frame if it has a sprite.
        if (s2d_ecs_entity_has(aniEID, CMP_TYPE_SPRITE)) {
            Component* sprCmp = s2d_ecs_get_component(aniEID, CMP_TYPE_SPRITE);
            sprCmp->sprite.frame =
                animationCmp->animation.animation->frames[(u64) animationCmp->animation.aniIndex];
        }
    }
}

void system_fps(f32 timeStep) {
}
