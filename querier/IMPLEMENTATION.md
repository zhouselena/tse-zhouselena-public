# CS50 TSE Querier
## Implementation Spec

This document references the [Design Specification](DESIGN.md).

- Data structures
-  Control flow: pseudo code for overall flow, and for each function
-  Detailed function prototypes and their parameters
-  Error handling and recovery

## Data structures and other modules

`twoCounters`: this is a local type that holds two counter sets. This is made to be passed into iterator functions that can only hold one `void*` type.

`countersAndInt`: this is a local type that holds a counter set and an int. This is made to be passed into iterator functions that can only hold one `void*` type.

We leverage the modules of libcs50, most notably `index`, `counters`, `file`, and `webpage`.
See that directory for more detailed module interfaces.

## Control flow

### main

The `main` function validates the arguments, processes each query until user quits, finds the websites that match each query, and prints it out for the user to see.

`main` calls `cleanQuery` and `splitWords` to process the query.

`main` calls `calculateScores` to return a counter set of all matching documents, and `printScores` to print each matching doc in ranked order.

### cleanQuery

The `cleanQuery` function takes a line and normalizes it, returning a normalized and cleaned query line. If it contains any non-alpha characters, it returns NULL and prints a bad character error message.

### splitWords

The `splitWords` function takes a clean query line and splits it into an array of words. It assigns a start and end pointer to the query line and moves the end pointer until a whitespace is reached, then adds that word to the array.

### calculateScores

The `calculateScores` function calculates the score of all matching documents based on word precedence. It iterates over an array of strings, and if there is no "and/or" present, takes the intersection of the left total and current word. If there is an "or", it calculates the total from the right word to the next "or" and adds to the total score.

### printScores

The function `printScores` prints the scores of the matching docs in ranked order. It calls `calculateNumbScores` to determine whether it should print a "No documents match" message. Otherwise, for each matching doc, it iterates over the counter set, prints the doc ID with the largest count, then sets that count to 0.

## Function prototypes

### querier

```c
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

```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by if statements, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit e.g., if a function receives bad parameters.
