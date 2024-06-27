#include <stoff2d_ecs.h>
#include <cds/cds_exlist.h>

#include <string.h>
#include <stdio.h>

/**************************** Component Hash Map *****************************/

/* component_map_init
 * ------------------
 * Initialise component map.
 */
void component_map_init(s2dComponentMap* map);

/* component_map_destroy
 * ---------------------
 * Cleanup memory.
 */
void component_map_destroy(s2dComponentMap* map);

/* component_map_size
 * ------------------
 * Retrieve the current size of the map.
 */
u64 component_map_size(s2dComponentMap* map);

/* component_map_put
 * -----------------
 * Insert a component into the map.
 */
void component_map_put(s2dComponentMap* map, Component cmp);

/* component_map_get
 * -----------------
 * Retrieve a reference to the component with eID. NULL if not found.
 */
Component* component_map_get(s2dComponentMap* map, u32 eID);


/* component_map_delete
 * --------------------
 * Delete a record from the map
 */
void component_map_delete(s2dComponentMap* map, u32 eID);

/* component_map_print
 * --------------------
 * Print the data values of the map.
 */
void component_map_print(s2dComponentMap* map);

/*****************************************************************************/


/***************************** ECS starts here *******************************/

// Component buckets. Array of hash maps mapping <eID, Component>
s2dComponentMap componentBuckets[CMP_TYPE_COUNT];

// Each ComponentType indexes to it's corresponding bitmask.
u64 componentMasks[CMP_TYPE_COUNT];

// Each eID is it's bitflag which indicates which components it has.
u64 componentFlags[S2D_MAX_ENTITIES];

// Stack of eIDs that are reused in ecs_create_entity. 
// First eID is 1 since 0 is reserved as an error/empty eID.
cdsExList* recycledIDs;
u32        nextID = 1;

// ComponentStrings. (only used for debug printing)
const char* componentStrings[CMP_TYPE_COUNT] = {
    "PositionComponent",
    "SpriteComponent",
    "VelocityComponent"
};

/****************************** ADD/REMOVE ***********************************/

u32 s2d_ecs_create_entity() {
    u32 eID = NO_ENTITY;
    if (cds_exlist_len(recycledIDs)) {
        eID = *((u32*) cds_exlist_pop(recycledIDs));
    } else if (nextID == S2D_MAX_ENTITIES) {
        fprintf(stderr,
                "Exceeded S2D_MAX_ENTITIES, consider increasing in settings.h\n");
    } else {
        eID = nextID++;
    }
    return eID;
}

void s2d_ecs_delete_entity(u32 eID) {
    // Delete all the entitie's components.
    for (size_t type = 0; type < CMP_TYPE_COUNT; type++) {
        s2d_ecs_delete_component(eID, type);
    }

    // Recycle the eID.
    cds_exlist_push(recycledIDs, &eID);
}

void s2d_ecs_add_component(Component component) {
    u32 eID            = component.eID;
    ComponentType type = component.type;

    // Don't add the component if the entity already has it.
    if (componentFlags[eID] & componentMasks[type]) {
        return;
    }

    // Add it to it's correct bucket.
    component_map_put(&componentBuckets[type], component);
    
    // Update the bitflag.
    componentFlags[eID] |= componentMasks[type];
}

void s2d_ecs_delete_component(u32 eID, ComponentType type) {
    // Don't attempt to delete a component that doesn't exist.
    if (componentFlags[eID] & componentMasks[type]) {
        component_map_delete(&componentBuckets[type], eID);
        componentFlags[eID] ^= componentMasks[type];
    }

}

Component* s2d_ecs_get_component(u32 eID, ComponentType type) {
    return (Component*) component_map_get(&componentBuckets[type], eID);
}

s2dComponentMap* s2d_ecs_get_bucket(ComponentType type) {
    return &componentBuckets[type];
}

bool s2d_ecs_entity_has(u32 eID, ComponentType type) {
    return (componentFlags[eID] & componentMasks[type]);
}

/********************************** DEBUG ************************************/

void s2d_ecs_print_components() {
    for (ComponentType type = 0; type < CMP_TYPE_COUNT; type++) {
        s2dComponentMap* components = &componentBuckets[type];
        printf("\nCOMPONENTS: %s (size - %lu tableSize - %lu)\n",
                componentStrings[type], 
                component_map_size(components),
                s2d_component_map_tablesize(components));
        component_map_print(components);
        printf("END\n");
    }
}

/**************************** INIT/SHUTDOWN **********************************/

void s2d_ecs_initialise() {
    // Component masks.
    memset(componentMasks, 0, sizeof(componentMasks));
    componentMasks[0] = 0x00000001;
    for (size_t i = 1; i < CMP_TYPE_COUNT; i++) {
        componentMasks[i] = componentMasks[i - 1] << 1;
    }

    // Component flags.
    memset(componentFlags, 0, sizeof(componentFlags));

    // Recycled eIDs.
    recycledIDs = cds_exlist_create(sizeof(u32), cds_cmpu);

    // Component buckets.
    for (size_t i = 0; i < CMP_TYPE_COUNT; i++) {
        component_map_init(&componentBuckets[i]);
    }
}

void s2d_ecs_shutdown() {
    // Recycled eIDs.
    cds_exlist_destroy(recycledIDs);

    // Component Buckets.
    for (size_t i = 0; i < CMP_TYPE_COUNT; i++) {
        component_map_destroy(&componentBuckets[i]);
    }
}
