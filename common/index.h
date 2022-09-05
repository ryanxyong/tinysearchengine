/*
 * index.h - header file for 'index' module
 *
 * An 'index' is a hashtable with words as the keys and counters sets as their
 * items.
 *
 * Ryan Yong, May 2021
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

/************* global types ************/
typedef hashtable_t index_t; // visible to module users

/************* index_new ************/
/*
 * Initializes a new index
 */
index_t* index_new(const int num_slots);

/************* index_add *************/
/*
 * Adds a word with its corresponding docID or increments the count if the
 * word and docID already exist in the index.
 */
bool index_add(index_t* index, char* word, int docID);

/************* indexdir_init ****************/
/*
 * Initializes a new index output file if the path is writeable
 */
bool indexdir_init(const char* indexFilename);

/************* index_validate ***************/
/*
 * Checks if an index filename is readable
 */
bool index_validate(const char* indexFilename);

/************* ctrs_docCount **************/
/*
 * Helper function for doc_print to print docID and count pairs
 */
void ctrs_docCount(void* indexFilename, const int docID, const int count);

/************** doc_print ***************/
/*
 * Helper function for index_save to print the word and docID and count pairs
 */
void doc_print(void* indexFilename, const char* word, void* ctrs);

/************* index_save *************/
/*
 * Saves all contents of an index into a given indexFilename
 */
void index_save(index_t* index, const char* indexFilename);

/************** word_delete ***************/
/*
 * Deletes the counters for a word
 */
void word_delete(void* item);

/************* index_delete ***************/
/*
 * Deletes the entire index, including itself
 */
void index_delete(index_t* index);

/************* read_index ************/
/*
 * Takes an old index file and returns a new inverted index.
 */
index_t* read_index(char* indexFilename);