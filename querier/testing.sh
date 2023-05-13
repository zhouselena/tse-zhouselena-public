#!/bin/bash
#
# testing.sh: testing file for 'querier'

# **** Invalid tests **** #

# Incorrect number of arguments
./querier
./querier ../data/pgdir/letters
./querier ../data/pgdir/letters ../data/indexer/pgdir-letters-index extrainput

# Wrong page directory
./querier wrong_page_directory ../data/indexer/pgdir-letters-index
./querier ../common ../data/indexer/pgdir-letters-index

# Wrong index file
./querier ../data/pgdir/letters wrong_index_file

# Empty index file
./querier ../data/pgdir/letters ../data/indexer/broken

# **** Valid tests with fuzzquery **** #

# local directory

./fuzzquery ../data/indexer/pgdir-letters-index 10 2 | ./querier ../data/pgdir/letters ../data/indexer/pgdir-letters-index

./fuzzquery ../data/indexer/wikipedia_2-index 8 99 | ./querier ../data/pgdir/wikipedia ../data/indexer/wikipedia_2-index

./fuzzquery ../data/indexer/toscrape-1-index 3 1 | ./querier ../data/pgdir/toscrape ../data/indexer/toscrape-1-index

# cs50 output - toscrape

./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 2 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index

./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 7 4 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index