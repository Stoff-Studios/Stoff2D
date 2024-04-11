#include <component_map.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// TODO:- Use an actual hash function.
//      - Quadratic probing instead of linear.
//      - Use a static array of primes as table sizes for resizing.


#define INIT_TABLE_SIZE 11
#define MAX_LOAD_FACTOR 0.75f

u32 hash_eid(u32 eID) {
    return eID; 
}

void component_map_clear(Entry* data, u64 tableSize) {
    for (u64 i = 0; i < tableSize; i++) {
        Entry* e = &data[i];
        e->active = false;
        e->tombStoned = false;
        memset(&e->component, 0, sizeof(Component));
    }
}

void resize(ComponentMap* map) {
    u64 newTableSize = map->tableSize * 2;
    Entry* newData = malloc(sizeof(Entry) * newTableSize);
    component_map_clear(newData, newTableSize);
    for (u64 i = 0; i < map->tableSize; i++) {
        Entry entry = map->data[i];
        if (!entry.active) {
            continue;
        }
        // Prepare our rehashed entry.
        Component c = entry.component;
        u64 hash = hash_eid(c.eID);
        u64 newIndex = hash % newTableSize;
        Entry newEntry = (Entry) {
            .component = c,
            .active = true,
            .tombStoned = false
        };
        // Linear probe until we hit a free spot.
        while (newData[newIndex].active) {
            newIndex++;
            if (newIndex >= newTableSize) {
                newIndex = 0;
            }
        }
        newData[newIndex] = newEntry;
    }
    free(map->data);
    map->data = newData;
    map->tableSize = newTableSize;
} 

void component_map_init(ComponentMap* map) {
    map->size = 0;
    map->tableSize = INIT_TABLE_SIZE;
    map->data = malloc(sizeof(Entry) * INIT_TABLE_SIZE);
    component_map_clear(map->data, INIT_TABLE_SIZE);
}

u64 component_map_size(ComponentMap* map) {
    return map->size;
}

u64 component_map_tablesize(ComponentMap* map) {
    return map->tableSize;
}

void component_map_put(ComponentMap* map, Component cmp) {
    if (((f32) map->size / (f32) map->tableSize) > MAX_LOAD_FACTOR) {
        resize(map);
    }
    Entry entry = (Entry) {
        .component = cmp,
        .active = true,
        .tombStoned = false
    };
    u64 index = hash_eid(cmp.eID) % map->tableSize;
    // Linear probe until we hit a free spot.
    u64 tombStoneIndex; 
    bool foundTombStoneSlot = false;
    while (map->data[index].active) {
        // Key already exists, overwrite.
        if (map->data[index].component.eID == cmp.eID) {
            map->data[index] = entry;
            return;
        }
        // Found a free tombstoned slot.
        if (map->data[index].tombStoned && !foundTombStoneSlot) {
            tombStoneIndex = index;
            foundTombStoneSlot = true;
        }
        index++;
        index = index % map->tableSize;
    }

    if (foundTombStoneSlot) {
        map->data[tombStoneIndex] = entry;
    } else {
        map->data[index] = entry;
    }
    map->size++;
}

Component* component_map_get(ComponentMap* map, u32 eID) {
    u64 startIndex = hash_eid(eID) % map->tableSize;
    for (u64 i = 0; i < map->tableSize; i++) {
        u64 nextIndex = (startIndex + i) % map->tableSize;
        if (map->data[nextIndex].component.eID == eID) {
            return &map->data[nextIndex].component;
        }
    }
    return NULL;
}

Component* component_map_at(ComponentMap* map, u64 index) {
    if (index < 0 || index >= map->tableSize) {
        return NULL;
    }
    return &map->data[index].component;
}

void component_map_delete(ComponentMap* map, u32 eID) {
    u64 startIndex = hash_eid(eID) % map->tableSize;
    for (u64 i = 0; i < map->tableSize; i++) {
        u64 nextIndex = (startIndex + i) % map->tableSize;
        if (map->data[nextIndex].component.eID == eID) {
            map->data[nextIndex].component.eID = NO_ENTITY;
            map->data[nextIndex].active = false;
            map->data[nextIndex].tombStoned = true;
            map->size--;
            break;
        }
    }
}

void component_map_destroy(ComponentMap* map) {
    free(map->data);
}

void component_map_print(ComponentMap* map) {
    for (u64 i = 0; i < map->tableSize; i++) {
        printf("eID: %u | active: %u | tombStoned: %u\n", 
                map->data[i].component.eID,
                map->data[i].active,
                map->data[i].tombStoned);
    }
}
