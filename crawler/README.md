# 'crawler' directory for Tiny Search Engine

### Selena Zhou, CS50 23W

> `common` and `libcs50` libraries need to be made before running `crawler`.

### Crawler
> https://github.com/CS50spring2023/home/blob/main/knowledge/units/crawler.md

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number of hops from the seed URL, and avoiding visiting any given URL more than once. It saves the pages, and the URL and depth for each, in files. When the crawler process is complete, the indexing of the collected documents can begin.

### Compilation
To make, run `make crawler` in terminal.
To test valgrind, run `make valgrind` in terminal.
To test with `testing.sh`, run `make test` in terminal.

### Usage
Run crawler with `./crawler seedURL pageDirectory maxDepth` where `seedURL`
is the website to crawl, `pageDirectory` is an existing empty directory to store
results in, and `maxDepth` is the number of levels deep to crawl the `seedURL`. '

`crawler` exit code meanings:
* exit(0): successful crawl, no errors
* exit(1): incorrect number of args, caught in main
* exit(2): incorrect input, caught in parseArgs
* exit(3): incorrect memory allocation, caught in crawler

### Files:
* `.gitignore`: will ignore data written into `pgdir` directory, and file made by crawler
* `Makefile`: makes terminal by running command `make crawler` in terminal.
* `crawler.c`: crawler file, implementation details written in comments.
* `testing.sh`: testing file
* `testing.out`: testing output file
