# Testing plan
# Unit testing. A program indextest will serve as a unit test for the index module;
# it reads an index file into the internal index data structure, then writes the index out to a new index file.

# Integration testing. The indexer, as a complete program, will be tested by building an index from a pageDirectory,
# and then the resulting index will be validated by running it through the indextest to ensure it can be loaded.

# 1. Test indexer with various invalid arguments. 
#   2. no arguments
#   3. one argument
#   4. three or more arguments
#   5. invalid pageDirectory (non-existent path)
#   5. invalid pageDirectory (not a crawler directory)
#   6. invalid indexFile (non-existent path)
#   7. invalid indexFile (read-only directory) 7. invalid indexFile (existing, read-only file)
# 2. Run indexer on a variety of pageDirectories and use indextest as one means of validating the resulting index.
# 3. Run valgrind on both indexer and indextest to ensure no memory leaks or errors.

./indexer ../data/pgdir/letters ../data/indexer/pgdir-letters-index
./indexer ~/cs50-dev/shared/tse/output/letters-2 ../data/indexer/letters-2-index
./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data/indexer/toscrape-1-index
./indexer ~/cs50-dev/shared/tse/output/wikipedia_1 ../data/indexer/wikipedia_2-index

./indextest ../data/indexer/pgdir-letters-index ../data/indexer/indextest/IT_pgdir-letters-index