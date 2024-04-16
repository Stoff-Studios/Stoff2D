#pragma once

#include <component_map.h>

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
ComponentMap* s2d_ecs_get_bucket(ComponentType type);

/* s2d_ecs_entity_has
 * ------------------
 * Return true if the entity with eID has a component of type.
 */
bool s2d_ecs_entity_has(u32 eID, ComponentType type);

/********************************* DEBUG *************************************/

/* ecs_print_components
 * --------------------
 * Print all the component buckets and their contents.
 */
void s2d_ecs_print_components();

/***************************** INIT/SHUTDOWN *********************************/

/* ecs_init
 * --------
 * Initialise the ecs.
 * NOTE: this is called by engine during intialisation, no need to call.
 */
void ecs_init();

/* ecs_shutdown
 * ------------
 * Free memory used by the ecs.
 * NOTE: this is called by engine during shutdown, no need to call.
 */
void ecs_shutdown();

