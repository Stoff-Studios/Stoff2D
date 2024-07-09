#pragma once

#include <defines.h>

/* To extend the ecs and add more components to the system
 *
 * 1. extend ComponentType enum with the new component type.
 * 2. create a struct for the new component
 * 3. add the new struct to the union in Component (at the bottom of this file)
 *
 * Now the new component will be in the system
 */

// eID of 0 is reserved as an empty value. 
#define NO_ENTITY 0 

// Component types.
typedef enum {
    CMP_TYPE_POSITION,
    CMP_TYPE_SPRITE,
    CMP_TYPE_VELOCITY,
    CMP_TYPE_PLAYER,
    CMP_TYPE_DEATH_TIMER,
    CMP_TYPE_PARTICLE_EMITTER,
    CMP_TYPE_ENEMY,
    CMP_TYPE_HEALTH,
    CMP_TYPE_DAMAGE,
    CMP_TYPE_HITBOX,
    CMP_TYPE_ANIMATION,
    CMP_TYPE_COUNT
} ComponentType;

typedef struct {
    clmVec2 position;
} PositionComponent;

typedef struct {
    clmVec2  size;
    clmVec4  colour;
    u32      texture;
    s2dFrame frame;
    u8       layer;
} SpriteComponent;

typedef struct {
    clmVec2 velocity;
    clmVec2 maxSpeed;
} VelocityComponent;

typedef struct {
    u8 TODO;
} PlayerComponent;

typedef struct {
    f32 timeLeft;
} DeathTimerComponent;

typedef struct {
    u32 particleType;
    f32 timeUntillNextEmit;
    f32 emitWaitTime;
} ParticleEmitterComponent;

typedef struct {
    u32 playerEID;
} EnemeyComponent;

typedef struct {
    f32 hp;
    f32 maxHp;
    f32 invinsibilityTimer;
    f32 invinsibilityTime;
} HealthComponent;

typedef struct {
    f32 damage;
    f32 cooldown;
    f32 currentCooldown;
    bool deleteOnHit;
} DamageComponent;

typedef struct {
    clmVec2 position;
    clmVec2 size;
} HitBoxComponent;

typedef struct {
    s2dAnimation* animation;
    f32           aniIndex;
    f32           aniSpeed;
} AnimationComponent;

typedef struct {
    u32           eID;  // ID of the entity the component belongs to.
    ComponentType type; // Type of this component.
    union {   
        PositionComponent        position;
        SpriteComponent          sprite;
        VelocityComponent        velocity;
        PlayerComponent          player;
        DeathTimerComponent      deathTimer;
        ParticleEmitterComponent particleEmitter;
        EnemeyComponent          enemy;
        HealthComponent          health;
        DamageComponent          damage;
        HitBoxComponent          hitbox;
        AnimationComponent       animation;
    };
} Component;
