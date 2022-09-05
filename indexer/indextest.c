/*
 * indextest.c - takes an index file, creates a new index data structure, and
 * produces a new index file with the new index data structure.
 *
 * usage - User must provide the old index filename and new (output) index
 * filename.
 *
 * Ryan Yong, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "counters.h"
#include "index.h"

/************* function prototypes **************/
static int parseArgs(const int argc, char* argv[],
                     char** oldIndexFilename, char** newIndexFilename);
static void make_index(char* oldIndexFilename, char* newIndexFilename);

/************* main **************/
int
main(const int argc, char* argv[])
{
  char* oldIndexFilename = NULL;
  char* newIndexFilename = NULL;

  parseArgs(argc, argv, &oldIndexFilename, &newIndexFilename);
  make_index(oldIndexFilename, newIndexFilename);
}

/************* parseArgs *************/
/*
 * Given user-inputted arguments, assigns them to variables and then returns
 * the exit status.
 */
static int
parseArgs(const int argc, char* argv[],
          char** oldIndexFilename, char** newIndexFilename)
{
  if (argc == 3) {
    *oldIndexFilename = argv[1];
    if (! index_validate(*oldIndexFilename)) { // Validate old index filename
      exit(1);
    }

    *newIndexFilename = argv[2];
    if (! indexdir_init(*newIndexFilename)) { // Validate new index filename
      exit(1);
    }
    return 0;
  } else {
    fprintf(stderr,
            "Invalid number of arguments.\nFormat: oldIndexFilename newIndexFilename\n");
    exit(1);
  }
}

/************* make_index ************/
/*
 * Takes an old index file, makes an inverted index, and creates a new index
 * file.
 */
static void
make_index(char* oldIndexFilename, char* newIndexFilename)
{
  index_t* index = read_index(oldIndexFilename);
  index_save(index, newIndexFilename);
  index_delete(index);
}