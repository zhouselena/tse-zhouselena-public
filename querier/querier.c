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


/**************** funct declarations ****************/

char* cleanQuery(char* word);

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

        // Prompt for query
        printf("Query: ");
        if ((searchLine = file_readLine(stdin)) == NULL) { // mallocs space
            printf("\n");
            break;      // end search if EOF
        }
        
        // clean query, print error or print cleaned query
        char* cleanedQuery = cleanQuery(searchLine);
        if (cleanedQuery == NULL) {
            continue;
        }
        printf("Evaluating query: %s\n", cleanedQuery);

        free(cleanedQuery); // frees space
        free(searchLine); // frees space

    }

}

/**************** functions ****************/

/* cleanQuery */
/* Takes in a char* word, and removes extra whitespaces or non-alpha characters and 
 * normalizes the word.
 * If word is empty, return NULL.
 * IF USED, NEEDS TO FREE
 */
char* cleanQuery(char* word) {

    char* cleanWord = malloc(strlen(word));     // mallocs space
    int cleanWordIndex = 0;
    int numbWhiteSpaces = 0;

    for (int i = 0; i < strlen(word); i++) {

        if (isspace(word[i])) {   // encountering a whitespace
            if (i != 0 && !isspace(word[i-1])) {     // add proper whitespaces
                cleanWord[cleanWordIndex] = word[i];
                cleanWordIndex++;
            }
            numbWhiteSpaces++;      // skip trailing or duplicate whitespaces
        }

        else if (!isalpha(word[i])) {    // encountering a non-alpha character
            printf("Error: bad character '%c' in query.\n", word[i]);
            free(cleanWord);
            return NULL;
        }

        else {  // encountering an alpha character
            cleanWord[cleanWordIndex] = tolower(word[i]);
            cleanWordIndex++;
        }

    }

    if (numbWhiteSpaces == strlen(word)) {
        printf("Error: empty query.\n");
        return NULL;
    }
    
    return cleanWord;

}