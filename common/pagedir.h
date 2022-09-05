/*
 * pagedir.h - header file for 'pagedir' module
 *
 * Pagedir initializes a page directory files and saves webpages
 *
 * Ryan Yong, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"

/***************** pagedir_init ****************/
/* 
 * Initializes the given page directory
 */
bool pagedir_init(const char* pageDirectory);

/************** pagedir_validate ***************/
/*
 * Checks if the given pageDirectory is a readable path
 */
bool pagedir_validate(const char* pageDirectory);

/***************** pagedir_save ****************/
/*
 * Saves a webpage to a given pageDirectory
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/***************** pagedir_load ******************/
/*
 * Returns the webpage associated with the given docID in the pageDirectory
 */
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/************** pagedir_getURL ****************/
/*
 * Returns the url associated with the given docID in the pageDirectory
 */
char* pagedir_getURL(const char* pageDirectory, int docID);