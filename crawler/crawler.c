/*
 * crawler.c
 * 
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

/**************** function declarations ****************/

static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);
static void deleteItem(void* item);

/**************** main ****************/
/* Calls parseArgs and crawl and exits 0.
 * 
 * Only calls if correct number of arguments passed, otherwise exits with code 1.
 * Other error calls will be caught and exit nonzero by parseArgs and crawl.
 * If all successful, main exits 0.
 */
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

    // If all successful, exit 0
    exit(0);

}

/**************** parseArgs ****************/
/* Given arguments from the command line, extract them into the function parameters.
 * Return only if successful.
 * 
 * For seedURL, normalizes the URL and validates it is an internal URL.
 * For pageDirectory, calls pagedir_init() and validates if true.
 * For maxDepth, ensures it is an integer in range [0, 10].
 * 
 * If any validation fails, exit with code 2.
 * Otherwise, return.
 */
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
    if (*maxDepth < 0 || *maxDepth > 10 || !strcmp("0", argv[3])) {
        fprintf(stderr, "maxDepth out of range [0, 10]\n");
        exit(2);
    }

}

/**************** crawl ****************/
/* Crawls from the seedURL to maxDepth, and saves pages in the pageDirectory.
 * 
 * Initializes hashtable to hold URLs, and initializes bag with seed webpage at depth 0.
 * While the bag is not empty and not at max depth,
 *      extracts webpage, fetches HTML, and saves page to pageDirectory.
 *      calls pageScan to scan the HTML and continue.
 * If unable to malloc space at for any given variable
 *      prints to stderr and exits with code 3.
 */
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
    hashtable_insert(seenURLs, seedURL, "");

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
            id++;
        }
        // delete that webpage
        webpage_delete(currentPage);
    }

    // delete the hashtable, delete the bag
    hashtable_delete(seenURLs, deleteItem);
    bag_delete(crawlBag, webpage_delete);

}

/**************** pageScan ****************/
/* Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs;
 * for any URL not already seen before (i.e., not in the hashtable),
 * add the URL to both the hashtable pages_seen and to the bag pages_to_crawl.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

    int pos = 0;
    char* nextPageURL;
    // while there is another URL in the page
    while((nextPageURL = webpage_getNextURL(page, &pos)) != NULL) {
        // if that URL is Internal, insert the webpage into the hashtable
        // if that succeeded, create a webpage_t for it, insert the webpage into the bag
        char* nextURL = normalizeURL(nextPageURL);
        if (isInternalURL(nextURL) && hashtable_insert(pagesSeen, nextURL, "")) {
            webpage_t* nextPage = webpage_new(nextURL, webpage_getDepth(page)+1, NULL);
            bag_insert(pagesToCrawl, nextPage);
            logr("Inserted", webpage_getDepth(nextPage), webpage_getURL(nextPage));
        }
        else {
            free(nextURL);
        }
        // free the URL
        free(nextPageURL);
        pos++;
    }
    
}

/**************** logr ****************/
/* Helper function to print log messages.
 * Log one word (1-9 chars) about a given url.
 */                                
static void logr(const char *word, const int depth, const char *url) {
    printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

/**************** deleteItem ****************/
/* Helper function to free items in hashtable.
 */
static void deleteItem(void* item) {
    if (item != NULL) {
        free(item);
    }
}