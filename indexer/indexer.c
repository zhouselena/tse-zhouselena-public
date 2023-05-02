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
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"

/**************** function declarations ****************/

static void parseArgs(const int argc, char* argv[], char* pageDirectory, char* indexFileName);
indexBuild
indexPage

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
    parseArgs(argc, argv, pageDirectory, indexFileName);

    // If all successful, exit 0
    exit(0);

}

/**************** parseArgs ****************/
/*
 * validate it received exactly two command-line arguments and that
 * pageDirectory is the pathname for a directory produced by the Crawler, and
 * indexFilename is the pathname of a file that can be written;
 */

static void parseArgs(const int argc, char* argv[], char* pageDirectory, char* indexFileName) {

}