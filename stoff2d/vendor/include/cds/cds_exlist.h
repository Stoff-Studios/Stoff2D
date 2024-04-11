#pragma once

/* cds_exlist.h
 *
 * A generic extensible list
 *
 * Written by Jesse Fryer
 */

#include "cds_utils.h"

// Opaque type.
typedef struct _ExList cdsExList;

/* cds_exlist_init
 * ---------------
 * Initialise an extensible list. Must be called before any other function.
 *
 * list: pointer to a cdsExList
 * dataSize: size (in bytes) of data type to be stored in the list 
 * cmp: function pointer to a compare function for the data type being stored
 *      in the list. See cds_utils.h for basic type implementations or make
 *      your own if you're using a custom data type.
 */
cdsExList* cds_exlist_create(size_t dataSize, cdsCmp cmp);

/* cds_exlist_push
 * ---------------
 * Insert an item to the back of the list. This function copies the data into
 * the list, and does nothing else with the pointer. So if it's dynamically 
 * allocated probs free it.
 *
 * list: pointer to a cdsExList
 * data: pointer to data to add, casted as a void pointer
 */
void cds_exlist_push(cdsExList* list, void* data);

/* cds_exlist_pop
 * --------------
 * Remove and return the last element from the list.
 *
 * list: pointer to a cdsExList.
 *
 * return: Make sure to dereference the pointer into whatever the datatype is
 *         pronto since the pointer will become invlaid on the next push.
 */
void* cds_exlist_pop(cdsExList* list);

/* cds_exlist_remove
 * -----------------
 * Find and remove data from the list.
 *
 * Returns NULL if a match for data was not found, otherwise returns a pointer
 * to the slot. 
 *
 * A match is determined by the compare function set in cds_exlist_create.
 */
void* cds_exlist_remove(cdsExList* list, void* data);

/* cds_exlist_clear
 * ----------------
 * Set len to 0. Doesn't actually do anything to the data.
 */
void cds_exlist_clear(cdsExList* list);

/* cds_exlist_insert
 * -----------------
 * Insert an element at an index. Inserting OOB will not do anything except
 * print a warning.
 */
void cds_exlist_insert(cdsExList* list, void* data, size_t idx);

/* cds_exlist_get
 * --------------
 * Return an element at index idx in a list
 *
 * list: pointer to a cdsExList
 * idx: index of element in the list
 *
 * return: the element at index idx in the list, NULL if out of bounds.
 *         Make sure to dereference into whatever type the element is since
 *         the pointer could become invalid on a subsequent push call.
 */
void* cds_exlist_get(cdsExList* list, size_t idx);

/* cds_exlist_find
 * ---------------
 * Find and return the first occurence of data in the list, NULL if not found.
 */
void* cds_exlist_find(cdsExList* list, void*);

/* cds_exlist_len
 * --------------
 * Return the length of the list.
 */
size_t cds_exlist_len(cdsExList* list);

/* cds_exlist_len
 * --------------
 * Return the capacity of the list.
 */
size_t cds_exlist_capacity(cdsExList* list);

/* cds_exlist_sort
 * ---------------
 * Sort the list.
 */
void cds_exlist_sort(cdsExList* list);

/* cds_exlist_destroy
 * ------------------
 * Free underlying array used by list. This will not free dynamically allocated
 * member variables of custom datatypes, a custom destructor will need to be
 * implemented in that case.
 *
 * list: pointer to a cdsExList
 */
void cds_exlist_destroy(cdsExList* list);
