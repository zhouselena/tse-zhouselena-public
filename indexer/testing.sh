# `indexer` directory testing file

# Invalid arguments

./indexer # no arguments
./indexer ../data/pgdir/letters # one argument
./indexer ../data/pgdir/letters ../data/pgdir/letters ../data/pgdir/letters ../data/pgdir/letters # 3+ arguments
./indexer ../nonexistent/letters ../data/indexer/pgdir-letters-index # non-existent pageDirectory
./indexer ../common ../data/indexer/pgdir-letters-index # not a crawler directory
./indexer ../data/pgdir/letters ../data/indexer/broken # non-existent file

# Valid arguments

./indexer ../data/pgdir/letters ../data/indexer/pgdir-letters-index

./indexer ~/cs50-dev/shared/tse/output/letters-2 ../data/indexer/letters-2-index

./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../data/indexer/toscrape-1-index

./indexer ~/cs50-dev/shared/tse/output/wikipedia_1 ../data/indexer/wikipedia_2-index

# indextest test

./indextest ../data/indexer/pgdir-letters-index ../data/indexer/indextest/IT_pgdir-letters-index

# Valgrind tests

valgrind ./indextest ../data/pgdir/letters ../data/indexer/pgdir-letters-index

valgrind ./indextest ../data/indexer/pgdir-letters-index ../data/indexer/indextest/it-pgdir-letters-index