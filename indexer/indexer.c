/*
 * indexer.c - builds and writes an index of word occurrences to a file
 *
 * usage - User must provide the page directory, the output for crawler, and
 * index filename, the desired output destination path.
 *
 * Ryan Yong, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "index.h"
#include "word.h"
#include "pagedir.h"
#include "file.h"


/************* function prototypes **************/
static int parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, const int docID);

/************* main **************/
int
main(const int argc, char* argv[])
{
  char* pageDirectory = NULL; // Initializes a pageDirectory character string
  char* indexFilename = NULL; // Initializes an indexFilename character string

  parseArgs(argc, argv, &pageDirectory, &indexFilename); // Handles arguments
  indexBuild(pageDirectory, indexFilename); // Handles most of the work

  return 0;
}

/************* parseArgs ***************/
/*
 * Given user-inputted arguments, assigns them to variables and then returns
 * the exit status.
 */
static int
parseArgs(const int argc, char* argv[],
          char** pageDirectory, char** indexFilename)
{
  // Checks if proper number of arguments were provided
  if (argc == 3) {
    *pageDirectory = argv[1];
    if (! pagedir_validate(*pageDirectory)) { // Validate page directory
      exit(1);
    }

    *indexFilename = argv[2];
    if (! indexdir_init(*indexFilename)) { // Validate index filename
      exit(1);
    }

    return 0;
  } else {
    fprintf(stderr,
            "Invalid number of arguments.\nFormat: pageDirectory indexFilename\n");
    exit(1);
  }
}

/************* indexBuild ************/
/*
 * Given the pageDirectory and indexFilename, creates an index into the
 * user-specified index filename.
 */
static void
indexBuild(char* pageDirectory, char* indexFilename)
{
  int docID = 1; // Starting docID

  const int num_slots = 600; // Defines number of slots for index
  index_t* index = index_new(num_slots);
  webpage_t* page = NULL;

  // Runs while a page can be loaded
  while ((page=(pagedir_load(pageDirectory, docID))) != NULL) {
    indexPage(index, page, docID++); // Indexes the current page's words
    webpage_delete(page); // Deletes the current page after indexing
  }

  index_save(index, indexFilename);
  index_delete(index);
}

/************* indexPage *************/
/*
 * Given an index, webpage, and docID, works through the given page to
 * add its words to the given index.
 */
static void
indexPage(index_t* index, webpage_t* page, const int docID)
{
  // Checks if the index and page are not null
  if (index != NULL && page != NULL) {
    char* word = NULL;
    int pos = 0; // Starting position in page
    // Loops while there is a next word
    while ((word=webpage_getNextWord(page, &pos)) != NULL) {
      if (strlen(word) >= 3) { // Skips words shorter than 3 characters
        normalizeWord(word); // Converts letters to lowercase

        index_add(index, word, docID);
      }

      free(word);
    }
  }
}
