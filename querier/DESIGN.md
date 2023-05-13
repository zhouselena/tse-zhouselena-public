# CS50 TSE Querier
## Design Spec

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

This document references the [Implementation Specification](IMPLEMENTATION.md).

- User interface
- Inputs and outputs
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Testing plan

### User interface

The querier's interface with the user is first on the command-line, which must always have two arguments.

```
./querier pageDirectory indexFilename
```

If the command is valid, querier will prompt the user for a query through `Query: `. Once the user inputs a query, which must only contain alpha characters without any starting or ending or adjacent and/or's, it will print out the cleaned query line with `Evaluating query: [cleaned query]`.

### Inputs and outputs

**Inputs:** the querier takes in a page directory with documents to search from, and an index file that contains all the word counts for each document ID.

The querier reads lines from stdin until the user quits/reached EOF.

**Output:** the querier prints out a list of matching website links ranked by score.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validates parameters, then creates index module from the given information.
    while has input:
        prompt user for query
        clean query
        create array of words, process word logic
        calculate scores of docs that match the words
        print matching documents ranked by score

### Testing plan

The `querier`, as a complete program, will be tested through a testing file `testing.sh` that passes in various arguments. `testing.sh` uses `fuzzquery` to generate random query lines to input into `querier`.

1. Test `querier` with various invalid arguments.
    * no arguments
    * one argument
    * three or more arguments
    * invalid `pageDirectory`, non-existent path
    * invalid `pageDirectory`, not a crawler directory
    * invalid index file, not an index file
    * invalid index file, empty index file
2. Run `querier` on a variety of page directories, using `fuzzQuery` to generate random queries.
3. Run `valgrind` on `indexer` tests to ensure no memory leaks or errors.
