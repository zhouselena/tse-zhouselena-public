/*
 * pagedir.h
 * Header file for pagedir.c
 *
 * Selena Zhou, CS50 23S
 *
 */

bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);