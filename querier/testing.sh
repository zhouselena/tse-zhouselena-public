#!/bin/bash
#
# testing.sh: testing file for 'querier'

# **** Invalid tests **** #

# **** Test with fuzzquery **** #

# local directory
./fuzzquery ../data/indexer/pgdir-letters-index 10 2 | ./querier ../data/pgdir/letters ../data/indexer/pgdir-letters-index

# to-scrape
./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 2 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index
./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 7 4 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index
