/*
 * word.c - 'word' module
 *
 * see word.h for more information
 *
 * Ryan Yong, May 2021
 */

 #include <string.h>
 #include <ctype.h>
 #include <stdlib.h>
 #include "word.h"

/************* normalizeWord **************/
/* see word.h for description */
char*
normalizeWord(char* word)
{
  // char* result = malloc((strlen(word)+1)*sizeof(char));

  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
  return word;
}