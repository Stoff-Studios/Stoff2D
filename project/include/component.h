#pragma once

#include <defines.h>

// 0 is reserved as error value
#define NO_ENTITY 0 

// Component types.
typedef enum {
    CMP_TYPE_POSITION,
    CMP_TYPE_SPRITE,
    CMP_TYPE_VELOCITY,
    CMP_TYPE_BOX_COLLIDER,
    CMP_TYPE_CONTROL,
    CMP_TYPE_COUNT
} ComponentType;

typedef struct {
    clmVec2 position;
} PositionComponent;

typedef struct {
    clmVec2 size;
    u32     texture;
    Frame   frame;
    u8      layer;
} SpriteComponent;

typedef struct {
    clmVec2 velocity;
} VelocityComponent;

typedef struct {
    f32 width;
    f32 height;
} BoxColliderComponent;

typedef struct {
    bool lmao;
} ControlComponent;


typedef struct {
    u32           eID;  // ID of the entity the component belongs to.
    ComponentType type; // Type of this component.
    union {   
        PositionComponent    position;
        SpriteComponent      sprite;
        VelocityComponent    velocity;
        BoxColliderComponent boxCollider;
        ControlComponent     control;
    };
} Component;
