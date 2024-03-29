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

/**************** local types ****************/

typedef struct twoCounters { // for counter_iterate w multiple counters
    counters_t* first;
    counters_t* second;
} twoCounters_t;

typedef struct countersAndInt { // for counter_iterate w multiple counters
    counters_t* counter;
    int* intKey;
} countersAndInt_t;


/**************** function declarations ****************/

/* formatting query */

char* cleanQuery(char* word);
void splitWords(char* query, char** words, int numWords);
int numWords(char* query);
int checkQueryLogic(char** words, int numwords);
void printCleanedQuery(char** words, int numwords);

/* calculating doc scores */

counters_t* calculateScores(index_t* dex, char** words, int numwords);
counters_t* copyCounter(counters_t* counter);
counters_t* wordAndWord(counters_t* firstWord, counters_t* secondWord);
counters_t* wordOrWord(counters_t* firstWord, counters_t* secondWord);
int calculateNumbScores(counters_t* scores);
void printScores(counters_t* scores, char* pageDirectory);

/* iterator helper functions */

void copyCounterHelper(void* arg, const int key, const int count);
void countersHelper_wordAndWord(void* arg, const int key, const int count);
void countersHelper_wordOrWord(void* arg, const int key, const int count);
void countersHelper_calculateNumbScores(void* arg, const int key, const int count);
void countersHelper_findMax(void* arg, const int key, const int count);
void countersHelper_printScores(char* pageDirectory, const int key, const int count);

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
    if (dex == NULL) {
        fprintf(stderr, "Error loading index.\n");
        fclose(indexFile);
        exit(3);
    }
    index_load(dex, indexFile);
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
        counters_t* scores = calculateScores(dex, words, numwords);
        printScores(scores, pageDirectory);

        counters_delete(scores);
        free(cleanedQuery); // frees space
        free(searchLine); // frees space

    }

    index_delete(dex);

}

/**************** functions ****************/

/**************** formatting query ****************/

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
            fprintf(stderr, "Error: bad character '%c' in query.\n", word[i]);
            free(cleanWord);
            return NULL;
        }

        else {  // encountering an alpha character
            cleanWord[cleanWordIndex] = tolower(word[i]);
            cleanWordIndex++;
        }

    }

    if (numbWhiteSpaces == strlen(word)) {
        fprintf(stderr, "Error: empty query.\n");
        free(cleanWord);
        return NULL;
    }
    
    return cleanWord;

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

/* checkQueryLogic */
/* Takes an array of words and checks that and/or logic is correct.
 * Returns 0 if correct
 * Returns 1 if incorrect.
 */
int checkQueryLogic(char** words, int numwords) {

    // first word can't be and/or
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
        fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
        return 1;
    }
    
    // last word can't be and/or
    if (strcmp(words[numwords-1], "and") == 0 || strcmp(words[numwords-1], "or") == 0) {
        fprintf(stderr, "Error: '%s' cannot be last\n", words[numwords-1]);
        return 1;
    }

    // check for adjacent logic
    for (int i = 1; i < numwords; i++) {
        // if current word is and/or
        if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
            // if previous word was also and/or
            if (strcmp(words[i-1], "and") == 0 || strcmp(words[i-1], "or") == 0 ) {
                fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i-1], words[i]);
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

/**************** calculating doc scores ****************/

/* calculateScores */
/* Given an index and array of words, calculates the score of all matching docs.
 * Calculates based on word precedence.
 *  If no and/or present, takes the intersection.
 *  If and present, skips and and takes the intersection.
 *  If or present, calculates right side total with precedence and adds to overall total.
 * Returns a NEWLY MALLOC'd counter set of docIDs and scores.
 * USER MUST FREE
 */

counters_t* calculateScores(index_t* dex, char** words, int numwords) {

    counters_t* totalScores = copyCounter(index_get(dex, words[0]));
    int i = 1;

    while (i < numwords) {

        if (strcmp(words[i], "and") == 0) {     // skip and
            i++;
            continue;
        } else if (strcmp(words[i], "or") == 0) {
            i++;
            counters_t* rightTotal = copyCounter(index_get(dex, words[i]));
            while (((i < numwords-2) && strcmp(words[i+1], "or") != 0)) {      // while haven't reached another OR or end of words, calculates right side
                if (strcmp(words[i], "and") == 0) {     // skip and
                    i++;
                    continue;
                }
                counters_t* currentWord = index_get(dex, words[i]);
                counters_t* result = wordAndWord(rightTotal, currentWord);
                counters_delete(rightTotal);
                rightTotal = result;
                i++;
            }
            counters_t* result = wordOrWord(totalScores, rightTotal); // once reached end or next OR, add right total to total
            counters_delete(rightTotal);
            counters_delete(totalScores);
            totalScores = result;
            i++;
        }
        else {  // normal word without and/or in between, default to and
            counters_t* currentWord = index_get(dex, words[i]);
            counters_t* result = wordAndWord(totalScores, currentWord);
            counters_delete(totalScores);
            totalScores = result;
            i++;
        }

    }

    return totalScores;

}

/* copyCounters */
/* Creates a clean copy of a counter set for safe free'ing.
 */
counters_t* copyCounter(counters_t* counter) {
    counters_t* copyTo = counters_new();
    counters_iterate(counter, copyTo, copyCounterHelper);
    return copyTo;
}

/* wordAndWord */
/* Given two counter sets, returns the intersection of the two sets.
 * Returns a new malloc'd counterset.
 * USER NEEDS TO FREE
 */
counters_t* wordAndWord(counters_t* firstWord, counters_t* secondWord) {
    counters_t* and = counters_new();
    twoCounters_t secondAndResult = {and, secondWord};
    counters_iterate(firstWord, &secondAndResult, countersHelper_wordAndWord);
    return and;
}

/* wordOrWord */
/* Given two counter sets, returns the union of the two sets.
 * Returns a new malloc'd counterset.
 * USER NEEDS TO FREE
 */
counters_t* wordOrWord(counters_t* firstWord, counters_t* secondWord) {
    counters_t* or = counters_new();
    if (or == NULL) return NULL;
    counters_iterate(firstWord, or, countersHelper_wordOrWord);
    counters_iterate(secondWord, or, countersHelper_wordOrWord);
    return or;
}

/* calculateNumbScores */
/* Given a counterset, counts how many items the counterset stores.
 * Used to determine whether to print a "no docs match" output.
 * Returns count.
 */
int calculateNumbScores(counters_t* scores) {
    int numscores = 0;
    counters_iterate(scores, &numscores, countersHelper_calculateNumbScores);
    return numscores;
}

/* printScores */
/* Prints scores of all matching docs in ranked descending order.
 */
void printScores(counters_t* scores, char* pageDirectory) {
    int numscores = calculateNumbScores(scores);
    if (numscores > 0) {
        printf("Matches %d documents (ranked):\n", numscores);
        for (int i = 0; i < numscores; i++) {
            int previousScoreKey = -1;
            countersAndInt_t info = {scores, &previousScoreKey};
            counters_iterate(scores, &info, countersHelper_findMax);
            countersHelper_printScores(pageDirectory, previousScoreKey, counters_get(scores, previousScoreKey));
            counters_set(scores, previousScoreKey, 0); // set previous max to 0
        }
    } else {
        printf("No documents match.\n");
    }
    printf("-----------------------------------------------\n");
}

/**************** iterator helper functions ****************/

/* copyCounterHelper */
/* Helper function for copyCounter.
 * Adds counter to new counter set.
 */
void copyCounterHelper(void* arg, const int key, const int count) {
    counters_t* copyTo = arg; // cast to counters
    counters_set(copyTo, key, count);
}

/* countersHelper_wordAndWord */
/* Helper function for wordAndWord.
 * Determines whether docID has a second word,
 * then sets score to the minimum of the two scores..
 */
void countersHelper_wordAndWord(void* arg, const int key, const int count) {
    twoCounters_t* secondAndResult = arg; // cast to two counters
    int secondCount = counters_get(secondAndResult->second, key);
    if (secondCount > 0) {          // if docID exists in second word
        if (secondCount < count) {  // if there is less count in second word
            counters_set(secondAndResult->first, key, secondCount);
        } else {
            counters_set(secondAndResult->first, key, count);
        }
    }
}

/* countersHelper_wordOrWord */
/* Helper function for wordOrWord.
 * Adds counter to overall scores without duplicating docIDs.
 */
void countersHelper_wordOrWord(void* arg, const int key, const int count) {
    counters_t* scores = arg; // cast arg to scores counter
    counters_set(scores, key, counters_get(scores, key) + count);   // will not duplicate scores but rather add on top
}

/* countersHelper_calculateNumbScores */
/* Helper function for calculateNumbScores.
 * Adds 1 to numscores.
 */
void countersHelper_calculateNumbScores(void* arg, const int key, const int count) {
    int* numscores = arg; // cast to int pointer
    *numscores += 1;
}

/* countersHelper_findMax */
/* Helper function for printScores.
 * If the count is larger, replaces the max counter.
 */
void countersHelper_findMax(void* arg, const int key, const int count) {
    countersAndInt_t* info = arg; // case to key
    if (counters_get(info->counter, *info->intKey) < count) {
        *info->intKey = key;
    }
}

/* countersHelper_printScores */
/* Helper function for printScores.
 * Prints the score and URL of the counter.
 */
void countersHelper_printScores(char* pageDirectory, const int key, const int count) {
    char* docpathname = malloc(strlen(pageDirectory) + 12);
    sprintf(docpathname, "%s/%d", pageDirectory, key);
    FILE* doc = fopen(docpathname, "r");
    char* URL = file_readLine(doc);
    printf("score %3d doc %3d: %s\n", count, key, URL);
    free(docpathname);
    fclose(doc);
    free(URL);
}
