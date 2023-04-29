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