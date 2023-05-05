/*
 * pagedir.h - header file for CS50 'pagedir' module
 *
 * pagedir handles the page saver function that save pages to the target directory,
 * and marking it as a Crawler-produced pageDirectory.
 *
 * Selena Zhou, CS50 23S
 *
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/* Initializes .crawler file.
 * 
 * Constructs the pathname for the .crawler file in the given pageDirectory,
 * and then opens the file for writing.
 * On error:
 *      return false.
 * Otherwise:
 *      Close the file and return true.
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Saves a page to the page directory.
 * 
 * Constructs the pathname for the page file in the given pageDirectory,
 * and then opens the file for writing.
 * 
 * In the file,
 *      prints the URL, depth, and contents of the webpage.
 * Closes the file.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

bool pagedir_validate(const char* pageDirectory);

// webpage_t* pagedir_load(const char *pageDirectory, int docID);

#endif // __PAGEDIR_H