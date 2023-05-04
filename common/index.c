/* 
 * index.c - CS50 'index' module
 * 
 * see index.h for more information.
 * 
 *
 * Selena Zhou, 23S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/**************** global types ****************/

typedef struct index index_t {
    int num_slots;
    hashtable_t* table;
} index_t;

/**************** local functions ****************/


/**************** implemented functions ****************/

index_t* index_new(const int num_slots) {

    if (num_slots <= 0) {       // must have at least one slot
        return NULL;
    }

    // allocate memory
    index_t* dex = malloc(sizeof(index_t));
    dex->num_slots = num_slots;
    dex->table = hashtable_new(num_slots);

    if (dex == NULL) {
        return NULL;        // memory is improperly allocated
    }

}

void index_add(index_t* dex, char* word, const int docID) {
    
    if (dex == NULL || word == NULL || docID == NULL) {
        return;
    }
    
    hashtable_insert(dex->table, word, NULL);           // adds word to hashtable if not added already

    counters_t* currentCounter = hashtable_find(dex->table, word);

    if (currentCounter == NULL) {       // first occurence of the word
        counters_t* newCounter = counters_new();
        counters_add(newCounter, docID);
        hashtable_insert(dex->table, newCounter);
        // create a new slot in the hashtable with the word as the key
        // add a new counter in that slot
    } else {
        counters_add(currentCounter, docID);    // increment counter with docID
    }

}

void index_set(index_t* dex, char* word, const int docID, const int count) {
    
    if (dex == NULL || word == NULL || docID == NULL) {
        return;
    }

    hashtable_insert(dex->table, word, NULL);           // adds word to hashtable if not added already

    counters_t* currentCounter = hashtable_find(dex->table, word);

    if (currentCounter == NULL) {       // not been added already
        counters_t* newCounter = counters_new();
        counters_set(newCounter, docID, count);
        hashtable_insert(dex->table, newCounter);
        // create a new slot in the hashtable with the word as the key
        // add a new counter in that slot
    } else {
        counters_set(currentCounter, docID, count);    // change count of docID
    }

}

void index_load(index_t* dex, FILE* fp) {

    if (dex == NULL || fp == NULL) return;

    char* word;
    int docID;
    int count;
    while ((word = file_readWord(fp)) != NULL) {
        while ((fscanf(fp, "%d %d ", &docID, &count)) != 0) {
            index_set(dex, word, docID, count);
        }
        free(word);
    }

}

void counter_iterate_helper(void* arg, const int key, const int count) {

    FILE* fp = arg;                                             // cast to file
    fprintf(fp, " %d %d", key, count);

}

void hashtable_iterate_helper(void* arg, const char* key, void* item) {

    FILE* fp = arg;                                             // cast to file
    fprintf(fp, "%s", key);                                     // print word
    counters_t* ctrs = item;                                    // cast to counters
    counters_iterate(ctrs, counter_iterate_helper);
    fprintf(fp, "\n")                               // add new line after every word

}

bool index_save(index_t* dex, FILE* fp) {

    if (dex == NULL || fp == NULL) return false;

    hashtable_iterate(dex->table, fp, hashtable_iterate_helper);
    return true;

}

void delete_counter(void *item) {
    counters_t* ctr = item;
    counters_delete(ctr);
}

void index_delete(index_t* dex) {

    hashtable_delete(index->ht, delete_counter);
    free(dex);

}