# CS50 TSE Indexer
## Implementation Spec

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

`index`: Index is an index holding a hashtable word type (word, (docID, count)) that counts the number of times a word appears in each website. The size of the hashtable (slots) is impossible to determine in advance, so we use 300.

## Control flow

Indexer is implemented in `indexer.c` with the following functions:

### main

The `main` function validates the arguments passed from the command line, then calls indexBuild and index_save. Afterwards, it deletes the index and closes the file before exiting.

### indexBuild

The `indexBuild` function goes through each page in the pageDirectory until there are no more, loads the page through `pagedir_load`, and calls indexPage. Then it frees the space taken by the page.

### indexPage

The `indexPage` function goes through each line of the page, normalizes the word, then adds each word to the index and counts how many times the word appears in the page.

## Other modules

### index

Index is a type used by indexer. More detailed information is in the comments in the header `index.h` file and so will not be repeated here.

### pagedir

Re-usable module `pagedir.c` handles all page-related functions and is in the `../common` directory.

Pseudocode for `pagedir_validate`:

	construct the pathname for the crawler file and open the file for reading; on error, return false.
	then construct the pathname for the first document in the page directory with docID 1,
	open the file for reading, on error, return false.
	if all succeeded, return true.


Pseudocode for `pagedir_load`:

	construct pathname for the document in the page directory and open for reading; on error, return false.
	read the first few lines to get enough information to construct a new webpage, then fetch the HTML for that webpage.
	return the new webpage.

### libcs50

We leverage the modules of libcs50, most notably `bag`, `hashtable`, `file`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so our crawler need not implement that part of the spec.

## Function prototypes

### indexer

```c
int main(const int argc, char* argv[]);
static void indexBuild(index_t** dex, char* pageDirectory);
static void indexPage(index_t** dex, webpage_t* page, const int docID);
```

### index
```c
typedef struct index index_t;
index_t* index_new(const int num_slots);
void index_add(index_t* dex, char* word, const int docID);
void index_set(index_t* dex, char* word, const int docID, const int count);
void index_load(index_t* dex, FILE* fp);
bool index_save(index_t* dex, FILE* fp);
void index_delete(index_t* dex);
```

### pagedir

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char *pageDirectory, int docID);
```

### words
```c
char* normalizeWord(char* word);
```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by if statements, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit e.g., if a function receives bad parameters.

## Testing plan

Test cases are used to validate `indexer` and `indextest`.

### Unit testing.

A program indextest will serve as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

### Integration testing.

The indexer, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the indextest to ensure it can be loaded.

### Implemented tests.

Indexer is tested with:

Invalid arguments:
1. No arguments
2. One argument
3. Three or more arguments
4. invalid pageDirectory (non-existent path)
5. invalid pageDirectory (not a crawler directory)
6. invalid indexFile (non-existent path)

Valid arguments:
* Local directories created by crawler
* Directories given in `~/cs50-dev/shared/tse/`
* `indextest` runs to validate the resulting index (if no error messages print out, then it works!)
* Valgrind is run on both indexer and indextest to ensure no memory leaks or errors.