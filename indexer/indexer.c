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

static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static void indexBuild(index_t** dex, char* pageDirectory);
static void indexPage(index_t** dex, webpage_t* page, const int docID);

/**************** main ****************/

int main(const int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // Call parseArgs, will exit in parseArgs given invalid input
    char* pageDirectory;
    char* indexFileName;
    parseArgs(argc, argv, &pageDirectory, &indexFileName);

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

/**************** parseArgs ****************/
/*
 * validate it received exactly two command-line arguments and that
 * pageDirectory is the pathname for a directory produced by the Crawler, and
 * indexFilename is the pathname of a file that can be written;
 */

static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName) {

    // for pageDirectory, call pagedir_init()
    *pageDirectory = argv[1];
    if (pagedir_init(*pageDirectory) != true) {
        fprintf(stderr, "Not valid page directory\n");
        exit(2);
    }

    *indexFileName = argv[2];
    if (*indexFileName == NULL) {
        fprintf(stderr, "Not valid index file name\n");
        exit(2);
    }

}

/*where indexBuild:

  creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage*/

static void indexBuild(index_t** dex, char* pageDirectory) {

    *dex = index_new(300);       // hardcoded between 200 and 900

    char* pathname;
    FILE* fp;
    
    int docID = 1;
    while (true) {

        pathname = malloc(strlen(pageDirectory)+1+1+10);
        sprintf(pathname, "%s/%d", pageDirectory, docID);
        fp = fopen(pathname, "r");

        if (fp == NULL) break;

        printf("%s\n", pathname);   // track progress

        char* url = file_readLine(fp);
        char* depth_string = file_readLine(fp);
        char* html = file_readLine(fp);
        int depth = atoi(depth_string);              // cast depth to int

        webpage_t* page = webpage_new(url, depth, NULL);
        if (page != NULL && webpage_fetch(page) != false) {
            indexPage(dex, page, docID);
        }

        webpage_delete(page);
        free(pathname);
        free(depth_string);
        free(html);
        fclose(fp);
        docID++;

    }
    free(pathname);

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
        // printf("%s, %d\n", word, pos);
        normalizeWord(word);
        index_add(*dex, word, docID);
        free(word);
    }

}