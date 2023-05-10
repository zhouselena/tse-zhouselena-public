/*
 * querier.c
 * The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer,
 * and page files produced by the TSE Querier, and answers search queries submitted via stdin.
 * 
 * More detailed information on implementation and design can be found IMPLEMENTATION.md and DESIGN.md.
 *
 * Selena Zhou, CS50 23S
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"


/**************** function declarations ****************/

/**************** main ****************/

int main(const int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n");
        exit(1);
    }

    // Validates arguments
    char* pageDirectory = argv[1];
    char* indexFileName = argv[2];
    FILE* indexFile;
    if (pageDirectory == NULL || indexFileName == NULL || !pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Failed pagedir_validate\n");
        exit(2);
    }
    if ((indexFile = fopen(indexFileName, "r")) == NULL) {
        fprintf(stderr, "Unable to open index file\n");
        exit(2);
    }

    // Save index
    index_t* dex = index_new(file_numLines(indexFile));
    index_load(dex, indexFile);
    
    // Read queries
    char* searchLine;

    while(true) {

        if ((searchLine = file_readLine(stdin)) == NULL) { // mallocs space
            break;      // end search if EOF
        }

        printf("%s\n", searchLine);
        free(searchLine);

    }

}

/**************** functions ****************/
