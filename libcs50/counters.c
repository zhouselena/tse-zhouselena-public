/* 
 * counters.c - CS50 'counter' module
 *
 * see counters.h for more information.
 *
 * Selena Zhou, 23S
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"

/**************** local types ****************/
typedef struct counternode {
    int key;        // int key
    int count;       // count
    struct counternode *next;
} counternode_t;

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;       // head of the list of counters in set
} counters_t;

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t* new_counternode(int key) {
  counternode_t* node = malloc(sizeof(counternode_t));
  if (node == NULL) {     // error allocating memory for node; return error
    return NULL;
  }
  node->key = key;
  node->count = 0;
  return node;
}

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** FUNCTION ****************/
/* see counters.h for description */

counters_t* counters_new(void) {
    counters_t* counterset = malloc(sizeof(counters_t));
    if (counterset == NULL) {               // error allocating memory for node; return error
        return NULL;
    } else {
        counterset->head = NULL;            // initializes start of set
        return counterset;
    }
}

/**************** counters_add ****************/
/* see counters.h for description */

int counters_add(counters_t* ctrs, const int key) {
    
    if (ctrs != NULL && key >= 0) {
        if (counters_get(ctrs, key) == 0) {          // if key doesn't alr exist
            counternode_t* insert = new_counternode(key);
            insert->count = 1;
            insert->next = ctrs->head;
            ctrs->head = insert;
            return 1;
        } else {                                        // increment existing val
            for (counternode_t* currNode = ctrs->head; currNode != NULL; currNode = currNode->next) {
                if (currNode->key == key) {
                    currNode->count += 1;
                    return currNode->count;
                }
            }
        }
    }
    return 0;
}

/**************** counters_get ****************/
/* see counters.h for description */

int counters_get(counters_t* ctrs, const int key) {
    if (ctrs != NULL && key >= 0) {
        for (counternode_t* currNode = ctrs->head; currNode != NULL; currNode = currNode->next) {
            if (currNode->key == key) {
                return currNode->count;
            }
        }
    }
    return 0;
}

/**************** counters_set ****************/
/* see counters.h for description */

bool counters_set(counters_t* ctrs, const int key, const int count) {
    if (ctrs != NULL && key >=0 && count >=0) {
        if (counters_get(ctrs, key) == 0) {              // if key doesn't alr exist
            counternode_t* insert = new_counternode(key);
            insert->count = 1;
        } else {                                         // change existing val
            for (counternode_t* currNode = ctrs->head; currNode != NULL; currNode = currNode->next) {
                if (currNode->key == key) {
                    currNode->count = count;
                    return true;
                }
            }
        }
    }
    return false;
}

/**************** counters_print ****************/
/* see counters.h for description */

void counters_print(counters_t* ctrs, FILE* fp) {
    if (fp != NULL && ctrs != NULL) {
        fputc('{', fp);
        for (counternode_t* currNode = ctrs->head; currNode != NULL; currNode = currNode->next) {
            fprintf(fp, "Key %d: %d", currNode->key, currNode->count);
            // Fixes comma at end of list
            if (currNode->next != NULL) {
                fputc(',', fp);
            }
        }
        fputc('}', fp);
        fputs("\n", fp);
    } else {
        fputs("(null)\n", fp);
    }
}

/**************** counters_iterate ****************/
/* see counters.h for description */

void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count)) {
    if (ctrs != NULL && itemfunc!= NULL) {
        for (counternode_t* currNode = ctrs->head; currNode != NULL; currNode = currNode->next) {
            (*itemfunc)(arg, currNode->key, currNode->count);
        }
    }
}

/**************** counters_delete ****************/
/* see counters.h for description */

void counters_delete(counters_t* ctrs) {
    if (ctrs != NULL) {
        for (counternode_t* currNode = ctrs->head; currNode != NULL; ) {
            counternode_t* nextNode = currNode->next;
            free(currNode);
            currNode = nextNode;
        }
        free(ctrs);
    }
}