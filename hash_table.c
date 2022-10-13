/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Chiu-Chun, Chen
 * Email: chenchiu@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include "dynarray.h"
#include "list.h"
#include "hash_table.h"
#define HT_DEF_CAP  512

/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */ 
struct ht {
    int number_elements;
    int cap, mask;
    struct list **head;
    struct list **new_head;
};

int ht_cmp(void* a, void* b, int (*convert)(void*)) {
    int hindex1 = convert(a);
    int hindex2 = convert(b);

    if (hindex1 == hindex2) {
        return 0;
    }

    return 1;
}

/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create() {
    int i;

    struct ht *new_ht = malloc(sizeof(struct ht));
    if (new_ht == NULL) {
        return NULL;
    }
    new_ht->new_head = NULL;

    new_ht->number_elements = 0;
    new_ht->cap = HT_DEF_CAP;
    new_ht->head = malloc(new_ht->cap * sizeof(struct list *));
    if (new_ht->head == NULL) {
        free(new_ht);
        return NULL;
    }

    new_ht->mask = new_ht->cap - 1;

    for (i = 0; i < new_ht->cap; i++) {
        new_ht->head[i] = list_create();
    }

    return new_ht;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht) {
    int i;
    struct list *lhead;

    if (ht == NULL) {
        return;
    }

    for (i = 0; i < ht->cap; i++) {
        lhead = ht->head[i];
        list_free(lhead);
    }
    if (ht->head)       free(ht->head);
    if (ht->new_head)   free(ht->new_head);

    free(ht);

    return;
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht) {
    if (ht->number_elements == 0) {
        return 1;
    }

    return 0;
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 */
int ht_size(struct ht* ht) {
    return ht->number_elements;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash algorithm is totally up to you. Make sure to consider
 * Determinism, Uniformity, and Speed when design the hash algorithm
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element to be stored
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)) {
    int hindex = convert(key);

    return hindex & ht->mask;
}

/*
 * This function should insert a given element into a hash table with a
 * specified key.  Note that you cannot have two same keys in one hash table.
 * This function is passed a function pointer that is used to convert the key (void*)
 * to a unique hashcode (int).
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

int ht_enqueue(void *ht, void *key, void* value, int (*convert)(void*)) {
    struct ht* my_ht = (struct ht* )ht;
    int hindex = ht_hash_func(my_ht, key, convert);
    struct list *lhead = my_ht->head[hindex];

    list_insert(lhead, key, value);
    my_ht->number_elements++;
    return 0;
}

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)) {
    int i, j, load_factor, old_cap;
    int hindex = ht_hash_func(ht, key, convert);
    struct list *lhead = ht->head[hindex];
    struct list **old_head;

    if (ht_lookup(ht, key, convert) != NULL) {
        return;
    }

    list_insert(lhead, key, value);
    ht->number_elements++;

    load_factor = ht->number_elements / ht->cap;
    if (load_factor >= 4) {
        ht->number_elements = 0;
        old_cap = ht->cap;
        old_head = ht->head;

        ht->cap <<= 1;
        ht->head = malloc(ht->cap * sizeof(struct list *));
        ht->mask = ht->cap - 1;

        for (i = 0; i < ht->cap; i++) {
            ht->head[i] = list_create();
        }

        for (i = 0; i < old_cap; i++) {
            lhead = old_head[i];
            list_remove_enqueue_all(ht, lhead, ht_enqueue, convert);
        }
        free(old_head);
    }

    return;
}


/*
 * This function should search for a given element in a hash table with a
 * specified key provided.
 * This function is passed a function pointer that is used to convert the key (void*)
 * to a unique hashcode (int).
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)) {
    void *val;
    int hindex = ht_hash_func(ht, key, convert);
    struct list *lhead = ht->head[hindex];

    val = list_find_val_by_key(lhead, key, ht_cmp, convert);

    return val;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.
 * This function is passed a function pointer that is used to convert the key (void*)
 * to a unique hashcode (int).
 * If the key is found, remove the element and return, otherwise, do nothing and return
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)) {
    int hindex = ht_hash_func(ht, key, convert);
    struct list *lhead = ht->head[hindex];

    list_remove_by_key(lhead, key, ht_cmp, convert);
    ht->number_elements--;

    return;
}

