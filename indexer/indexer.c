/*
 * indexer.c
 * The TSE indexer is a standalone program that
 * reads the document files produced by the TSE crawler,
 * builds an index,
 * and writes that index to a file.
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"


/**************** function declarations ****************/

static void indexBuild(index_t** dex, char* pageDirectory);
static void indexPage(index_t** dex, webpage_t* page, const int docID);

/**************** main ****************/

int main(const int argc, char* argv[]) {

    /*
     * validate it received exactly two command-line arguments and that
     * pageDirectory is the pathname for a directory produced by the Crawler, and
     * indexFilename is the pathname of a file that can be written;
     */

    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // Validates arguments
    char* pageDirectory = argv[1];
    char* indexFileName = argv[2];
    if (pageDirectory == NULL || indexFileName == NULL || !pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Failed pagedir_validate\n");
        exit(2);
    }

    // Call indexBuild
    index_t* dex;
    indexBuild(&dex, pageDirectory);

    // Create new file and print index to the file
    FILE* fp = fopen(indexFileName, "w");
    if (fp == NULL) {
        fprintf(stderr, "Unable to create new file\n");
        exit(1);
    }
    index_save(dex, fp);

    // If all successful, free, exit 0
    index_delete(dex);
    fclose(fp);

    exit(0);

}

/*where indexBuild:

  creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage*/

static void indexBuild(index_t** dex, char* pageDirectory) {

    *dex = index_new(300);       // hardcoded between 200 and 900

    webpage_t* page;
    int docID = 1;
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {

        // printf("%s/%d\n", pageDirectory, docID);   // track progress
        indexPage(dex, page, docID);
        webpage_delete(page);
        docID++;

    }

}

/*where indexPage:

 steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID*/

static void indexPage(index_t** dex, webpage_t* page, const int docID) {

    char* word;
    int pos = 0;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        normalizeWord(word);
        index_add(*dex, word, docID);
        free(word);
    }

}