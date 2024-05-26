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

#ifdef __cplusplus
extern "C" {
#endif

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
 * Retrieve the bucket for the component type. Each bucket is a hash map
 * mapping eID -> Component. Also use this for iteration over components
 * (see s2d_component_map_at)
 */
s2dComponentMap* s2d_ecs_get_bucket(ComponentType type);

/* s2d_ecs_entity_has
 * ------------------
 * Return true if the entity with eID has a component of type type.
 */
bool s2d_ecs_entity_has(u32 eID, ComponentType type);

/* s2d_component_map_tablesize
 * ---------------------------
 * Retrieve the current tablesize of the map. 
 *
 * Useful for iterating over all the components in a map as if it were an array. 
 * (see s2d_component_map_at)
 */
u64 s2d_component_map_tablesize(s2dComponentMap* map);

/* s2d_component_map_at
 * --------------------
 * Retrieve a reference to the component at index in the underlying array.
 * This allows the user to treat the hash map as an array and iterate over
 * all the components of a particular type.
 *
 * Generally, to iterate over all components of type X,
 *     
 *     s2dComponentMap* components = s2d_ecs_get_bucket(CMP_TYPE_X);
 *     for (int i = 0; i < s2d_component_map_tablesize(components); i++) {
 *         Component* component = s2d_component_map_at(components, i);
 *         if (component->eID == NO_ENTITY) {
 *             continue; // skip empty slots in the map.
 *         }
 *         ... // go ham.
 *     }
 *
 */
Component* s2d_component_map_at(s2dComponentMap* map, u64 index);

/* ecs_print_components
 * --------------------
 * Print all the component buckets and their contents for debugging purposes.
 */
void s2d_ecs_print_components();

#ifdef __cplusplus
}
#endif
