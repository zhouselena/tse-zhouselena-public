# 'querier' directory for Tiny Search Engine

### Selena Zhou, CS50 23S

> `common` and `libcs50` libraries need to be made before running `querier`.

### Indexer
> https://github.com/CS50Spring2023/home/blob/main/labs/tse/querier/REQUIREMENTS.md

According to the Querier Requirements Spec, the TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### Compilation
To make `querier`, run `make` in terminal.

To test valgrind, run `make valgrind` in terminal.

To test with `testing.sh`, run `make test` in terminal.

### Usage
Run `./querier pageDirectory indexFilename` where `pageDirectory` is a crawler-made directory that stores pages, and `indexFileName` is the index file.

`querier` exit code meanings:
* exit(0): successful index, no errors
* exit(1): incorrect number of args, caught in main
* exit(2): invalid page directory or indexFileName, caught in main
* exit(3): empty index file, caught in main

### Functionalities implemented

* prints set of documents that contains all words in the query
* supports 'and' and 'or' operators WITH precedence
* prints document set in decreasing order by score

Meets full specs.

### Assumptions
`querier` assumes that documents in the pageDirectory follow the pattern of ../pageDirectory/\[docID] starting at doc ID of 1 and no missing numbers in between.

`testing.sh` refers to local directories which are assumed to exist.

### Files:
* `.gitignore`: will ignore querier and core dumps
* `Makefile`: makes terminal by running command `make` in terminal.
* `querier.c`: querier file
* `IMPLEMENTATION.md`: implementation specifications
* `DESIGN.md`: design specifications
* `testing.sh`: testing file
* `testing.out`: testing output file
