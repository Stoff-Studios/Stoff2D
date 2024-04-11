/* cds_exlist.c
 *
 * Written by Jesse Fryer
 */

#include <cds/cds_exlist.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Initial capacity for an extensible list.
#define CDS_EL_INIT_CAP 4 

typedef struct _ExList {
    void*  data;      // pointer to data on the heap.
    size_t dataSize;  // size of each element in the data
    size_t len;       // current length of the list
    size_t capacity;  // total capacity of the list
    cdsCmp cmp;       // compare function used in sorting algorithms
} cdsExList;

void swap(cdsExList* list, size_t this, size_t that) {
    void* tmp = malloc(list->dataSize);
    void* thisPtr = cds_exlist_get(list, this);
    void* thatPtr = cds_exlist_get(list, that);
    memcpy(tmp, thisPtr, list->dataSize);
    memcpy(thisPtr, thatPtr, list->dataSize);
    memcpy(thatPtr, tmp, list->dataSize);
    free(tmp);
}

void bubblesort(cdsExList* list) {
    if (!list->len) {
        return;
    }

    size_t end = list->len - 1;
    while (end) {
        for (size_t i = 0; i < end; i++) {
            void* curr = cds_exlist_get(list, i);
            void* next = cds_exlist_get(list, i + 1);
            // curr is more than next so swap.
            if (list->cmp(curr, next) > 0) {
                swap(list, i, i + 1);
            }
        }
        end--;
    }
}

cdsExList* cds_exlist_create(
        size_t dataSize,
        cdsCmp cmp) {
    cdsExList* list = (cdsExList*) malloc(sizeof(cdsExList));
    list->capacity = CDS_EL_INIT_CAP;
    list->len = 0;
    list->dataSize = dataSize;
    list->cmp = cmp;
    list->data = malloc(CDS_EL_INIT_CAP * dataSize);
    return list;
}

void cds_exlist_push(cdsExList* list, void* data) {
    // Resize if necessary.
    if (list->len == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * list->dataSize);
    }
    // Copy bytes from data to next position in the list
    void* dest = ((char*) list->data) + (list->len * list->dataSize);
    memcpy(dest, data, list->dataSize);
    list->len++;
}

void* cds_exlist_pop(cdsExList* list) {
    if (!list->len) {
        return NULL;
    }
    // Get a pointer to the last element.
    void* removed = ((char*) list->data) + ((list->len - 1) * list->dataSize);
    list->len--;
    return removed;
}

void* cds_exlist_remove(cdsExList* list, void* data) {
    // Linear search through list until match is found.
    for (size_t i = 0; i < list->len; i++) {
        void* current = ((char*) list->data) + (i * list->dataSize);
        if (!list->cmp(data, current)) {
            // Match found, shift items to left.
            if (list->len > 1) { // Don't shift if len is 1.
                void* oneToTheRight = ((char*) current) + list->dataSize;
                size_t bytesToShift = (list->len - (i + 1)) * list->dataSize;
                memmove(current, oneToTheRight, bytesToShift);
            } else {
                memset(current, 0, list->dataSize); // 0 out for completion.
            }
            list->len--;
            return current;
        }
    }

    return NULL;
}

void cds_exlist_clear(cdsExList* list) {
    list->len = 0;
}

void cds_exlist_insert(cdsExList* list, void* data, size_t idx) {
    if (idx < 0 || idx > list->len - 1) {
        fprintf(stderr, 
                "[cds warning] attempted to insert OOB element in exlist\n");
        return;
    }
    void* dest = ((char*) list->data) + (idx * list->dataSize);
    memcpy(dest, data, list->dataSize);
}

void* cds_exlist_get(cdsExList* list, size_t idx) {
    if (idx < 0 || idx >= list->len)  {
        fprintf(stderr, 
                "[cds warning] attempted to access OOB element in exlist\n");
        return NULL;
    }
    return ((char*) list->data) + (idx * list->dataSize);
}

void* cds_exlist_find(cdsExList* list, void* data) {
    char* current = (char*) list->data;
    for (size_t i = 0; i < list->len; i++) {
        if (!list->cmp(current, data)) {
            return current;
        }
        current += list->dataSize; 
    }
    return NULL;
}

size_t cds_exlist_len(cdsExList* list) {
    return list->len;
}

size_t cds_exlist_capacity(cdsExList* list) {
    return list->capacity;
}

void cds_exlist_sort(cdsExList* list) {
    qsort(list->data, list->len, list->dataSize, list->cmp);
}

void cds_exlist_destroy(cdsExList* list) {
    free(list->data);
    free(list);
}
