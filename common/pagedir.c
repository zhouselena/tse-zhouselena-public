/*
 * pagedir.c
 * Refer to header file pagedir.h for more detailed function explanations.
 *
 * Selena Zhou, CS50 23S
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
bool pagedir_init(const char* pageDirectory) {

    // Create pathname for .crawler file
    char* pathname = calloc(strlen(pageDirectory) + strlen("/.crawler") + 10, 1);
    sprintf(pathname, "%s/.crawler", pageDirectory);

    // Check if can open file
    FILE* fp = fopen(pathname, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not create %s\n", pathname);
        free(pathname);
        return false;
    }

    // Close the file and return true
    fclose(fp);
    free(pathname);
    return true;

}

/**************** pagedir_save ****************/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    
    // construct the pathname for the page file in pageDirectory
    char* pathname = calloc(strlen(pageDirectory)+10, 1);
    sprintf(pathname, "%s/%d", pageDirectory, docID);

    // open that file for writing
    FILE* fp = fopen(pathname, "w+");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not create %s\n", pathname);
        free(pathname);
        return;
    }

    // print the URL, depth, and HTML content to the file
    fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    // close the file
    fclose(fp);
    free(pathname);

}

/**************** pagedir_validate ****************/
bool pagedir_validate(const char* pageDirectory) {

    // Catch null pageDirectory
    if (pageDirectory == NULL) return false;

    // Make sure pageDirectory exists and is a crawler directory with /.crawler file
    char* crawlerpathname = malloc(strlen(pageDirectory) + strlen("/.crawler") + 1 + 1);
    sprintf(crawlerpathname, "%s/.crawler", pageDirectory);

    FILE* crawlerfile = fopen(crawlerpathname, "r");
    if (crawlerfile == NULL) {
        fprintf(stderr, "Error: not a crawler directory, no pageDirectory/.crawler file\n");
        fclose(crawlerfile);
        free(crawlerpathname);
        return false;
    }

    fclose(crawlerfile);
    free(crawlerpathname);

    // Check if first document exists (docID 1)
    char* docpathname = malloc(strlen(pageDirectory) + strlen("/1") + 1 + 1);
    sprintf(docpathname, "%s/1", pageDirectory);

    FILE* docfile = fopen(docpathname, "r");
    if (docfile == NULL) {
        fprintf(stderr, "Error: no file with docID 1 in pageDirectory\n");
        fclose(docfile);
        free(docpathname);
        return false;
    }

    fclose(docfile);
    free(docpathname);

    return true;

}

/**************** pagedir_load ****************/
webpage_t* pagedir_load(const char *pageDirectory, int docID) {

    if (pageDirectory == NULL || docID < 1) return NULL;

    // Open file
    char* pathname = malloc(strlen(pageDirectory)+1+1+10);
    sprintf(pathname, "%s/%d", pageDirectory, docID);
    FILE* fp = fopen(pathname, "r");

    if (fp == NULL) {
        free(pathname);
        return NULL;
    }

    char* url = file_readLine(fp);
    char* depth_string = file_readLine(fp);
    char* html = file_readLine(fp);
    int depth = atoi(depth_string);              // cast depth to int

    webpage_t* page = webpage_new(url, depth, NULL);
    if (page == NULL) return NULL;
    webpage_fetch(page);

    free(pathname);
    free(depth_string);
    free(html);
    fclose(fp);

    return page;

}