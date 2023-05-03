/*
 * index.h - header file for CS50 'index' module
 *
 * The key data structure is the index, mapping from word to (docID, #occurrences) pairs.
 * The index is a hashtable keyed by word and storing counters as items.
 * The counters is keyed by docID
 * and stores a count of the number of occurrences of that word in the document with that ID.
 *
 * Selena Zhou, CS50 23S
 *
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/**************** global types ****************/
typedef struct index index_t;

/**************** functions ****************/

index_t* index_new(const int num_slots);
void index_add(index_t* dex, char* word, const int docID);
void index_set(index_t* dex, char* word, const int docID, const int count);
void index_load(index_t* dex, FILE* fp);
void index_save(index_t* dex, FILE* fp);
void counter_iterate_helper(void* arg, const int key, const int count);
void hashtable_iterate_helper(void* arg, const char* key, void* item);
void index_delete(index_t* dex);


#endif // __INDEX_H