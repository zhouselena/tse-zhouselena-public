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

/**************** index_new ****************/
/* Creates a new empty index.
 * 
 * Returns NULL if memory not correctly allocated.
 */
index_t* index_new(const int num_slots);

/**************** index_add ****************/
/* Adds a new (word, (docID, count)) pair to the index.
 * 
 * If the word already exists in the hashtable
 *  checks if counter already exists, then increments counter
 *  or creates a new counter for the docID if counter doesn't already exist and starts count at 1.
 */
void index_add(index_t* dex, char* word, const int docID);

/**************** index_set ****************/
/* Same mechanics as index_add, except sets the count to specified value.
 */
void index_set(index_t* dex, char* word, const int docID, const int count);

/**************** index_get ****************/
/* Gets the counter set of a specific word.
 */
counters_t *index_get(index_t *dex, const char *word);

/**************** index_load ****************/
/* Given an empty index and an index file, loads information from file to index.
 */
void index_load(index_t* dex, FILE* fp);

/**************** index_save ****************/
/* Given a created index, formats and prints the index information to the given file.
 * Return true if saved successfully.
 */
bool index_save(index_t* dex, FILE* fp);

/**************** index_delete ****************/
/* Frees all memory taken by index and deletes the index.
 */
void index_delete(index_t* dex);


#endif // __INDEX_H