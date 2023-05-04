/*
 * indextest.c
 * Index tester loads an index file produced by the indexer and saves it to another file.
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indexer.c"

/*
Unit testing. A program indextest will serve as a unit test for the index module;
it reads an index file into the internal index data structure, then writes the index out to a new index file.

Integration testing. The indexer, as a complete program, will be tested by building an index from a pageDirectory,
and then the resulting index will be validated by running it through the indextest to ensure it can be loaded.

execute from a command line with usage syntax
./indextest oldIndexFilename newIndexFilename
where oldIndexFilename is the name of a file produced by the indexer, and
where newIndexFilename is the name of a file into which the index should be written.
load the index from the oldIndexFilename into an inverted-index data structure.
create a file newIndexFilename and write the index to that file, in the format described below.
It need not validate its command-line arguments other than to ensure that it receives precisely two arguments; 
it may simply try to open the oldIndexFilename for reading and, later, try to open the newIndexFilename for writing. 
You may want to run this program as part of testing script that verifies that the output is identical to (or equivalent to) the input.

The index tester may assume that

The content of the index file follows the format specified below; 
thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

*/