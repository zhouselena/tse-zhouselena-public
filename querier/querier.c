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
int numWords(char* query);
void splitWords(char* query, char** words, int numWords);
int checkQueryLogic(char** words, int numwords);
void printCleanedQuery(char** words, int numwords);
int numDocs(char* pageDirectory);
void countersHelper_normalAdd(void* arg, const int key, const int count);
counters_t* calculateScores(index_t* dex, char** words, int numwords, int numdocs);
void countersHelper_calculateNumbScores(void* arg, const int key, const int count);
int calculateNumbScores(counters_t* scores);
void countersHelper_printScores(void* arg, const int key, const int count);
void printScores(counters_t* scores, char* pageDirectory);

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
    FILE* fp = fopen("./testing", "");
    index_save(dex, fp);
    fclose(indexFile);
    
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
        if (cleanedQuery == NULL) {     // invalid query
            free(cleanedQuery);
            free(searchLine);
            continue;
        }
        // split to an array of words
        int numwords = numWords(cleanedQuery);
        char* words[numwords];
        splitWords(cleanedQuery, words, numwords);
        // validate logic
        if (checkQueryLogic(words, numwords) == 1) {     // invalid logic
            free(cleanedQuery);
            free(searchLine);
            continue;
        }
        printCleanedQuery(words, numwords);   // only print if passes all tests

        // score documents
        // decide what to print
        int numdocs = numDocs(pageDirectory);
        counters_t* scores = calculateScores(dex, words, numwords, numdocs);
        printScores(scores, pageDirectory);

        free(scores);
        free(cleanedQuery); // frees space
        free(searchLine); // frees space

    }

    index_delete(dex);

}

/**************** functions ****************/

/* cleanQuery */
/* Takes in a char* word, and removes extra whitespaces or non-alpha characters and 
 * normalizes the word.
 * If word is empty, return NULL.
 * IF USED, NEEDS TO FREE
 */
char* cleanQuery(char* word) {

    char* cleanWord = calloc(strlen(word)+2, sizeof(char));     // mallocs space
    int cleanWordIndex = 0;
    int numbWhiteSpaces = 0;

    for (int i = 0; i < strlen(word); i++) {

        if (isspace(word[i])) {   // encountering a whitespace
            // skip leading white spaces, and only add space if next word exists (skips duplicate/trailing whitespaces)
            if (numbWhiteSpaces != i && isalpha(word[i+1])) {
                cleanWord[cleanWordIndex] = word[i];
                cleanWordIndex++;
            }
            numbWhiteSpaces++;
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
        free(cleanWord);
        return NULL;
    }
    
    return cleanWord;

}

/* numWords */
/* Takes a cleaned and safe query and returns the number of words in the query.
 */
int numWords(char* query) {
    int numWords = 1; // num words = num spaces + 1
    for (int i = 0; i < strlen(query); i++) {
        if (isspace(query[i])) numWords++;
    }
    return numWords;
}

/* splitWords */
/* Takes a cleaned and safe query and splits into an array of words.
 */
void splitWords(char* query, char** words, int numWords) {

    char* start = query;
    char* end = query;

    for (int i = 0; i < numWords; i++) {

        while(!isalpha(*start)) {   // move start to beginning of word
            start++;
        }

        end = start;
        while(isalpha(*end)) {     // move end to end of word
            end++;
        }

        *end = '\0';
        
        words[i] = start;    // add word to array
        start = end;        // move pointer to next word

    }

}

/* checkQueryLogic */
/* Takes an array of words and checks that and/or logic is correct.
 * Returns 0 if correct
 * Returns 1 if incorrect.
 */
int checkQueryLogic(char** words, int numwords) {

    // first word can't be and/or
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
        printf("Error: '%s' cannot be first\n", words[0]);
        return 1;
    }
    
    // last word can't be and/or
    if (strcmp(words[numwords-1], "and") == 0 || strcmp(words[numwords-1], "or") == 0) {
        printf("Error: '%s' cannot be last\n", words[numwords-1]);
        return 1;
    }

    // check for adjacent logic
    for (int i = 1; i < numwords; i++) {
        // if current word is and/or
        if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
            // if previous word was also and/or
            if (strcmp(words[i-1], "and") == 0 || strcmp(words[i-1], "or") == 0 ) {
                printf("Error: '%s' and '%s' cannot be adjacent\n", words[i-1], words[i]);
                return 1;
            }
        }

    }

    return 0;

}

/* printCleanedQuery */
/* Given an array of words, print the cleaned query.
 */
void printCleanedQuery(char** words, int numwords) {
    printf("Evaluating query:");
    for (int i = 0; i < numwords; i++) {
        printf(" %s", words[i]);
    }
    printf("\n");
}

/* numDocs */
/* counts the number of docs in a crawler directory */
int numDocs(char* pageDirectory) {
    int count = 0;
    char* docName = calloc(strlen(pageDirectory)+12, sizeof(char));
    int docID = 1;
    sprintf(docName, "%s/%d", pageDirectory, docID);
    FILE* fp;
    while ((fp = fopen(docName, "r")) != NULL) {
        count++;
        docID++;
        sprintf(docName, "%s/%d", pageDirectory, docID);
    }
    free(docName);
    return count;
}

void countersHelper_normalAdd(void* arg, const int key, const int count) {
    counters_t* scores = arg; // cast arg to scores counter
    counters_set(scores, key, counters_get(scores, key) + count);
}

counters_t* calculateScores(index_t* dex, char** words, int numwords, int numdocs) {

    /* Returns a counter with all matching docIDs and their scores
     * 
     * Scoring:
     * Get all counters of one word
     * Iterate over each docID and add the docID + score to the counter # figure out how to do and/or
     * Do this for each word
     * 
     * Ranking:
     * 
     */

    counters_t* scores = counters_new();

    for (int i = 0; i < numwords; i++) {

        counters_t* wordCounter = index_get(dex, words[i]);
        counters_iterate(wordCounter, scores, countersHelper_normalAdd);

    }

    return scores;

}

void countersHelper_calculateNumbScores(void* arg, const int key, const int count) {
    int* numscores = arg; // cast to int pointer
    *numscores += 1;
}

int calculateNumbScores(counters_t* scores) {
    int numscores = 0;
    counters_iterate(scores, &numscores, countersHelper_calculateNumbScores);
    return numscores;
}

void countersHelper_printScores(void* arg, const int key, const int count) {
    char* pageDirectory = arg; // cast to string
    char* docpathname = malloc(strlen(pageDirectory) + strlen("/1") + 1 + 1);
    sprintf(docpathname, "%s/key", pageDirectory);
    printf("score %*d doc %d: %s\n", 3, key, count, docpathname);
    free(docpathname);
}

void printScores(counters_t* scores, char* pageDirectory) {
    int numscores = calculateNumbScores(scores);
    if (numscores > 0) {
        printf("Matches %d documents (ranked):\n", numscores);
        counters_iterate(scores, pageDirectory, countersHelper_printScores);
    } else {
        printf("No documents match.\n");
    }
    printf("-----------------------------------------------\n");
}