#pragma once

#include <defines.h>

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
    CMP_TYPE_COUNT
} ComponentType;

typedef struct {
    clmVec2 position;
} PositionComponent;

typedef struct {
    clmVec2 size;
    clmVec4 colour;
    u32     texture;
    Frame   frame;
    u8      layer;
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
    u32           eID;  // ID of the entity the component belongs to.
    ComponentType type; // Type of this component.
    union {   
        PositionComponent        position;
        SpriteComponent          sprite;
        VelocityComponent        velocity;
        PlayerComponent          player;
        DeathTimerComponent      deathTimer;
        ParticleEmitterComponent particleEmitter;
    };
} Component;
