# Testing file for 'crawler'
# Expects pgdir/letters, pgdir/letters-b, pgdir/wikipedia, pgdir/toscrape to exist

#****** Erroneous cases ******#

# Invalid number of arguments
./crawler
./crawler 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters ../data/pgdir 1 1

# seedURL is invalid
./crawler http://hellohello.ca ../data/pgdir 2

# seedURL is not internal
./crawler https://www.google.ca/ ../data/pgdir 2

# pageDirectory does not exist
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ nopenope 2

# maxDepth out of range
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/pgdir 11

#****** Valid tests ******#

# Letters with uppercase URL before normalizing
./crawler HTTP://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/pgdir/letters 0

# Letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/pgdir/letters 6
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/B.html ../data/pgdir/letters-b 3

# Wikipedia
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/pgdir/wikipedia 1

# toScrape
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/pgdir/toscrape 1