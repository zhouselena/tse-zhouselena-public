/*
 * crawler.c
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int main(const int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }

    char** seedURL; char** pageDirectory; int* maxDepth;
    parseArgs(argc, argv, seedURL, pageDirectory, maxDepth); // will exit if not correct
    printf("Crawling %s with depth %d...\n", seedURL, maxDepth);
    return crawler(seedURL, pageDirectory, maxDepth);

}

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {

    // for seedURL, normalize the URL and validate it is an internal URL
    if (seedURL = normalize(argv[1]) == NULL || isInternalURL(seedURL) != true) {
        fprintf(stderr, "Not a valid URL\n");
        exit(2);
    }

    // for pageDirectory, call pagedir_init()
    pageDirectory = argv[2];
    if (pagedir_init(pageDirectory) != true) {
        fprintf(stderr, "Not valid page directory\n");
        exit(2);
    }

    // for maxDepth, ensure it is an integer in specified range
    maxDepth = atoi(argv[3]);
    if (maxDepth < 0 || maxDepth > 10 || (maxDepth == 0 && strcmp("0", argv[3]) != true)) {
        fprintf(stderr, "maxDepth out of range [0, 10]\n");
        exit(2);
    }

}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

    int pos = 0;
    char* nextpageURL;
    while((nextpageURL = webpage_getNextURL(page, &pos)) != NULL) {

        if (isInternalURL(nextPageURL)) {
            if (hashtable_insert(pagesSeen, nextPageURL, "")) {
                webpage_t* nextPage = webpage_new(nextPageURL, webpage_getDepth(page)+1, NULL);
                printf("Webpage inserted")
            }
        }

    }

    /*
    pageScan
This function implements the pagescanner mentioned in the design. Given a webpage, scan the given page to extract any links (URLs),
ignoring non-internal URLs; for any URL not already seen before (i.e., not in the hashtable), 
add the URL to both the hashtable pages_seen and to the bag pages_to_crawl. Pseudocode:

while there is another URL in the page
	if that URL is Internal,
		insert the webpage into the hashtable
		if that succeeded,
			create a webpage_t for it
			insert the webpage into the bag
	free the URL*/
}