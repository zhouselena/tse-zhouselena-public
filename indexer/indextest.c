/*
 * indextest.c
 * Index tester loads an index file produced by the indexer and saves it to another file.
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common/index.h"

int main(const int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        exit(1);
    }

    char* oldpathname = argv[1];
    FILE* oldIndexFile = fopen(oldpathname, "r");

    char* newpathname = argv[2];
    FILE* newIndexFile = fopen(newpathname, "w");

    if (oldIndexFile == NULL || newIndexFile == NULL) {
        fprintf(stderr, "Error opening files\n");
        exit(2);
    }

    index_t* dex = index_new(300);       // hardcoded between 200 and 900
    if (dex == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(3);
    }

    index_load(dex, oldIndexFile);
    index_save(dex, newIndexFile);
    index_delete(dex);
    fclose(oldIndexFile);
    fclose(newIndexFile);
    exit(0);                            // exit 0 on success

}