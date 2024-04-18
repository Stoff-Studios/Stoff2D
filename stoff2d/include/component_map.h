#pragma once

#include <defines.h>

typedef struct {
    Component component;
    bool      active;
    bool      tombStoned;
} Entry;

typedef struct _ComponentMap {
    Entry* data;
    u64    size;
    u64    tableSize;
} ComponentMap;

/* component_map_size
 * ------------------
 * Retrieve the current size of the map.
 */
u64 component_map_size(ComponentMap* map);

/* component_map_put
 * -----------------
 * Retrieve the current tablesize of the map.
 */
u64 component_map_tablesize(ComponentMap* map);

/* component_map_put
 * -----------------
 * Insert a component into the map.
 */
void component_map_put(ComponentMap* map, Component cmp);

/* component_map_get
 * -----------------
 * Retrieve a reference to the component with eID. NULL if not found.
 */
Component* component_map_get(ComponentMap* map, u32 eID);

/* component_map_at
 * ----------------
 * Retrieve a reference to the component at index in the underlying array.
 * Null entries have an eID of NO_ENTITY.
 */
Component* component_map_at(ComponentMap* map, u64 index);

/* component_map_delete
 * --------------------
 * Delete a record from the map
 */
void component_map_delete(ComponentMap* map, u32 eID);

/* component_map_print
 * --------------------
 * Print the data values of the map.
 */
void component_map_print(ComponentMap* map);
