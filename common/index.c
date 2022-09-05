/*
 * index.c - 'index' module
 *
 * see index.h for more information
 *
 * Ryan Yong, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"

/*************** global types *****************/
typedef hashtable_t index_t;

/*************** index_new ****************/
/* see index.h for description */
index_t*
index_new(const int num_slots)
{
  index_t* index = hashtable_new(num_slots);
  return index;
}

/*************** index_add ***************/
/* see index.h for description */
bool
index_add(index_t* index, char* word, const int docID)
{
  counters_t* searchItem = NULL;
  counters_t* tempCounter = NULL;

  // Checks if index or word are null
  if (index == NULL || word == NULL) {
    return false;
  }

  // Checks if the word is not found in the index
  if ((searchItem=(hashtable_find(index, word))) == NULL) {
    tempCounter = counters_new();
    counters_add(tempCounter, docID);
    hashtable_insert(index, word, tempCounter);
  } else { // Runs if the word is found in the index
    tempCounter = searchItem;
    counters_add(tempCounter, docID);
    hashtable_insert(index, word, tempCounter);
  }
  return true;
}

/************** indexdir_init ****************/
/* see index.h for description */
bool
indexdir_init(const char* indexFilename)
{
  char* path = malloc(strlen(indexFilename)+1); // Allocates memeory for path
  sprintf(path, "%s", indexFilename); // Writes pathname
  FILE* fp = NULL;

  // Checks if the file cannot be written
  if ((fp=fopen(path, "w")) == NULL) {
    fprintf(stderr, "Error: path \"%s\" is invalid.\n", path);
    free(path);
    return false;
  } else { // Runs if the file can be written
    fclose(fp);
    free(path);
    return true;
  }
}

/************** index_validate ***************/
/* see index.h for description */
bool
index_validate(const char* indexFilename)
{
  char* path = malloc(strlen(indexFilename)+1); // Allocates memeory for path
  sprintf(path, "%s", indexFilename); // Writes pathname
  FILE* fp = NULL;

  if ((fp=fopen(path, "r")) == NULL) { // Checks if file is readable at path
    fprintf(stderr, "Error: file \"%s\" does not exist.\n", path);
    free(path);
    return false;
  } else { // Runs if file is readable at path
    fclose(fp);
    free(path);
    return true;
  }
}

/************** ctrs_docCount **************/
/* see index.h for description */
void
ctrs_docCount(void* indexFilename, const int docID, const int count)
{
  fprintf(indexFilename, "%d %d ", docID, count);
}

/************** doc_print ***************/
/* see index.h for description */
void
doc_print(void* indexFilename, const char* word, void* ctrs) {
  fprintf(indexFilename, "%s ", word);
  // Used to print all docID and count pairs for a word
  counters_iterate(ctrs, indexFilename, ctrs_docCount);
  fprintf(indexFilename, "\n");
}

/************** index_save ***************/
/* see index.h for description */
void
index_save(index_t* index, const char* indexFilename)
{
  FILE* fp = NULL;

  if ((fp=fopen(indexFilename, "w")) != NULL) {
    // Used to move through each word in the index
    hashtable_iterate(index, fp, doc_print);
    fclose(fp);
  }
}

/************** word_delete ***************/
/* see index.h for description */
void
word_delete(void* item)
{
  counters_t* ctrs = item;
  counters_delete(ctrs);
}

/************** index_delete ***************/
/* see index.h for description */
void
index_delete(index_t* index)
{
  if (index != NULL) {
    hashtable_delete(index, word_delete);
  }
}

/************* read_index ************/
/* see index.h for description */
index_t*
read_index(char* indexFilename)
{
  char* path = malloc(strlen(indexFilename)+1); // Allocates memory for path
  sprintf(path, "%s", indexFilename); // Writes pathname
  FILE* fp = NULL;
  int num_slots = 0;
  
  // Checks if the index filename is readable
  if ((fp=fopen(path, "r")) != NULL) {
    num_slots = file_numLines(fp);
    index_t* index = index_new(num_slots);

    char* word = NULL;

    // Runs while there is another word in the file
    while ((word=file_readWord(fp)) != NULL) {
      counters_t* ctrs = counters_new();
      int docID = 0;
      int count = 0;
      
      // Runs while there is another docID and count pair
      while ((fscanf(fp, "%d %d", &docID, &count)) == 2) {
        counters_add(ctrs, docID);
        counters_set(ctrs, docID, count);
      }
      // Inserts the new counters set for a word into the index
      hashtable_insert(index, word, ctrs);
      free(word);
    }

    free(path);
    fclose(fp);
    return index;
  } else {
    fprintf(stderr, "Error: file \"%s\" does not exist.\n", path);
    free(path);
    return NULL;
  }
}