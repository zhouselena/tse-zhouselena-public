#!/bin/bash
#
# testing.sh: testing file for 'querier'

# Invalid tests

# Test with fuzzquery

./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 10 1 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index