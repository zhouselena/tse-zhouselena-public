/* 
 * word.c - CS50 'word' module
 * 
 * see word.h for more information.
 *
 * Selena Zhou, 23S
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* normalizeWord(char* word) {
    
    if (word == NULL) return NULL;
    
    for (int i = 0; i < strlen(word); i++) {
        word[i] = tolower(word[i]);
    }

    return word;

}
