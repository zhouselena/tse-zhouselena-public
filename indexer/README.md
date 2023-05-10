# 'indexer' directory for Tiny Search Engine

### Selena Zhou, CS50 23S

> `common` and `libcs50` libraries need to be made before running `indexer`.

### Indexer
> https://github.com/CS50Spring2023/home/blob/main/labs/tse/indexer/REQUIREMENTS.md

According to the Indexer Requirements Spec, the TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Compilation
To make `indexer` and `indextest`, run `make` in terminal.
To test valgrind, run `make valgrind` in terminal.
To test with `testing.sh`, run `make test` in terminal.

### Usage
Run `./indexer pageDirectory indexFilename` where `pageDirectory` is a crawler-made directory that stores pages, and `indexFileName` is the file to print the index to.

`indexer` exit code meanings:
* exit(0): successful index, no errors
* exit(1): incorrect number of args, caught in main
* exit(2): invalid page directory, caught in main
* exit(3): unable to create a file at indexFileName, caught in main

### Assumptions
`make test` assumes directories that already exist and ones that don't:
* pageDirectory docs are assumed to start at 1 and continue with no breaks
* ../data/indexer/broken is assumed to not exist

### Files:
* `.gitignore`: will ignore indexer and indextest, and core dumps
* `Makefile`: makes terminal by running command `make` in terminal.
* `indexer.c`: indexer file, implementation details written in IMPLEMENTATION.md
* `testing.sh`: testing file
* `testing.out`: testing output file
