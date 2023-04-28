/*
 * crawler.c
 * exit(1): incorrect number of args, caught in main
 * exit(2): incorrect input, caught in parseArgs
 * exit(3): incorrect memory allocation, caught in crawler
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/mem.h"

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);

int main(const int argc, char* argv[]) {

    // Check for correct number of arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }

    // Calls parseArgs and crawl
    char* seedURL;
    char* pageDirectory;
    int maxDepth;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth); // will exit if not correct
    printf("Crawling %s with depth %d...\n", seedURL, maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

}

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {

    // for seedURL, normalize the URL and validate it is an internal URL
    *seedURL = argv[1];
    char *URL = normalizeURL(*seedURL);
    if (URL == NULL || isInternalURL(URL) != true) {
        free(URL);
        fprintf(stderr, "Not a valid URL\n");
        exit(2);
    }
    free(URL);

    // for pageDirectory, call pagedir_init()
    *pageDirectory = argv[2];
    if (pagedir_init(*pageDirectory) != true) {
        fprintf(stderr, "Not valid page directory\n");
        exit(2);
    }

    // for maxDepth, ensure it is an integer in specified range
    *maxDepth = atoi(argv[3]);
    if (*maxDepth < 0 || *maxDepth > 10 /*|| (*maxDepth == 0 && strcmp("0", argv[3]) == false)*/) {
        fprintf(stderr, "maxDepth out of range [0, 10]\n");
        exit(2);
    }

}

static void crawl(char* seed, char* pageDirectory, const int maxDepth) {

    // initialize the hashtable and add the seedURL
    // initialize the bag and add a webpage representing the seedURL at depth 0
    char *seedURL = malloc(strlen(seed)+1);
    if (seedURL == NULL) {
        fprintf(stderr, "Could not malloc for seeURL\n");
        exit(3);
    }
    strcpy(seedURL, seed);

    webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
    bag_t* crawlBag = bag_new();
    hashtable_t* seenURLs = hashtable_new(100*(maxDepth+1));   // upper limit of 100 per page
    if (seedPage == NULL || crawlBag == NULL || seenURLs == NULL) {
        fprintf(stderr, "Failed to initialize bag or hashtable\n");
        exit(3);
    }

    bag_insert(crawlBag, seedPage);
    hashtable_insert(seenURLs, seedURL, seedPage);

    // while bag is not empty, pull a webpage from the bag
    webpage_t* currentPage;
    int id = 1;
    while ((currentPage = bag_extract(crawlBag)) != NULL) {
        // fetch the HTML for that webpage. if fetch was successful, save the webpage to pageDirectory
        if (webpage_fetch(currentPage)) {
            logr("Fetched", webpage_getDepth(currentPage), webpage_getURL(currentPage));
            pagedir_save(currentPage, pageDirectory, id);
            logr("Saved", webpage_getDepth(currentPage), webpage_getURL(currentPage));
            //if the webpage is not at maxDepth, pageScan that HTML
            if (webpage_getDepth(currentPage) != maxDepth) {
                logr("Scanning", webpage_getDepth(currentPage), webpage_getURL(currentPage));
                pageScan(currentPage, crawlBag, seenURLs);
            }
        }
        // delete that webpage
        webpage_delete(currentPage);
    }

    // delete the hashtable, delete the bag
    hashtable_delete(seenURLs, NULL);
    bag_delete(crawlBag, NULL);

}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

    int pos = 0;
    char* nextPageURL;
    // while there is another URL in the page
    while((nextPageURL = webpage_getNextURL(page, &pos)) != NULL) {
        // if that URL is Internal, insert the webpage into the hashtable
        // if that succeeded, create a webpage_t for it, insert the webpage into the bag
        if (isInternalURL(nextPageURL)) {
            if (hashtable_insert(pagesSeen, nextPageURL, "")) {
                webpage_t* nextPage = webpage_new(nextPageURL, webpage_getDepth(page)+1, NULL);
                bag_insert(pagesToCrawl, nextPage);
                logr("Webpage inserted", webpage_getDepth(nextPage), webpage_getURL(nextPage));
            }
        }
        // free the URL
        free(nextPageURL);
        pos++;
    }
    free(nextPageURL);
    
}

// log one word (1-9 chars) about a given url                                   
static void logr(const char *word, const int depth, const char *url) {
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}