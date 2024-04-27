#pragma once

#include <components.h>

typedef struct {
    Component component;
    bool      active;
    bool      tombStoned;
} s2dComponentEntry;

typedef struct {
    s2dComponentEntry* data;
    u64    size;
    u64    tableSize;
} s2dComponentMap;

/* ecs_create_entity
 * -----------------
 * Generate a new eID.
 */
u32 s2d_ecs_create_entity();

/* ecs_delete_entity
 * -----------------
 * Delete components associated with eID. eID gets put back in circulation.
 */
void s2d_ecs_delete_entity(u32 eID);

/* ecs_add_component
 * -----------------
 * Add a component to an entity.
 * If the entity already has an instance of the component type nothing happens.
 */
void s2d_ecs_add_component(Component component);

/* ecs_delete_component
 * --------------------
 * Remove a component from an entity.
 */
void s2d_ecs_delete_component(u32 eID, ComponentType type);

/* s2d_ecs_get_component
 * ---------------------
 * Retrieve a component from an entity. NULL if entity does not have it.
 */
Component* s2d_ecs_get_component(u32 eID, ComponentType type);

/* s2d_ecs_get_bucket
 * ------------------
 * Retrieve the bucket for the component type.
 */
s2dComponentMap* s2d_ecs_get_bucket(ComponentType type);

/* s2d_ecs_entity_has
 * ------------------
 * Return true if the entity with eID has a component of type.
 */
bool s2d_ecs_entity_has(u32 eID, ComponentType type);

/* s2d_component_map_tablesize
 * ---------------------------
 * Retrieve the current tablesize of the map. 
 *
 * Useful for iterating over all the components in a map as if it were an array. 
 * you will have to skip null entries which are signified by NO_ENTITY since.
 * (see s2d_component_map_at below)
 */
u64 s2d_component_map_tablesize(s2dComponentMap* map);

/* s2d_component_map_at
 * --------------------
 * Retrieve a reference to the component at index in the underlying array.
 * Null entries have an eID of NO_ENTITY.
 */
Component* s2d_component_map_at(s2dComponentMap* map, u64 index);

/* ecs_print_components
 * --------------------
 * Print all the component buckets and their contents for debugging purposes.
 */
void s2d_ecs_print_components();
