#include <stoff2d.h>

void system_render() {
    ComponentMap* spriteCmps = s2d_ecs_get_bucket(CMP_TYPE_SPRITE);
    for (u64 i = 0; i < component_map_tablesize(spriteCmps); i++) {
        Component* spriteCmp = component_map_at(spriteCmps, i);
        u32 eID = spriteCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }
        Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        s2d_sprite_renderer_add_sprite(
                spriteCmp->sprite, 
                posCmp->position.position);
    }
    s2d_sprite_renderer_render_sprites();
}

void system_move(f32 timeStep) {
    ComponentMap* velCmps = s2d_ecs_get_bucket(CMP_TYPE_VELOCITY);
    for (u64 i = 0; i < component_map_tablesize(velCmps); i++) {
        Component* velCmp = component_map_at(velCmps, i);
        u32 eID = velCmp->eID;
        if (eID == NO_ENTITY || !s2d_ecs_entity_has(eID, CMP_TYPE_POSITION)) {
            continue;
        }
        Component* posCmp = s2d_ecs_get_component(eID, CMP_TYPE_POSITION);
        posCmp->position.position.x += velCmp->velocity.velocity.x * timeStep;
        posCmp->position.position.y += velCmp->velocity.velocity.y * timeStep;
    }
}
