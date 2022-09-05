/*
 * querier.c - processes inputted queries and outputs matched documents
 *
 * usage - User must provide the page directory, the output for crawler, and
 * index filename, the output for indexer.
 *
 * Ryan Yong, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"
#include "counters.h"

/*************** local types ****************/
typedef struct docScore {
  int docID;
  int score;
} docScore_t;

/*************** function prototypes ***************/
static int parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void searchIndex(char* pageDirectory, char* indexFilename);
static int readQuery(char* input, char* words[], const int wordCount);
static bool queryCheck(index_t* index, char** words, int size);
static void resPrint(counters_t* res, char* pageDirectory);
static void resPrintHelper(void* arg, const int key, const int count);
static void countSort(docScore_t** docs, int size);
static docScore_t* docScore_new(int docID, int score);
static void andProcess(counters_t* total, counters_t* new);
static void andProcessHelper(void* arg, const int key, const int count);
static void orProcess(counters_t* total, counters_t* new);
static void orProcessHelper(void* arg, const int key, const int count);
static void ctrsMerge(counters_t* ctrs1, counters_t* ctrs2);
static void ctrsMergeHelper(void* arg, const int key, const int count);
static void ctrsCopy(counters_t* ctrs1, counters_t* ctrs2);
static void ctrsCopyHelper(void *arg, const int key, const int count);
static int ctrsDocsCount(counters_t* ctrs);
static void ctrsDocsCountHelper(void* arg, const int key, const int count);

/*************** main **************/
int
main (const int argc, char* argv[])
{
  char* pageDirectory = NULL;
  char* indexFilename = NULL;

  parseArgs(argc, argv, &pageDirectory, &indexFilename);
  searchIndex(pageDirectory, indexFilename);
}

/*************** parseArgs *****************/
/*
 * Given user-inputted arguments, assigns them to variables and then returns
 * the exit status.
 */
static int
parseArgs(const int argc, char* argv[],
          char** pageDirectory, char** indexFilename)
{
  if (argc == 3) {
    *pageDirectory = argv[1];
    if (! pagedir_validate(*pageDirectory)) { // Validate page directory
      exit(1);
    }

    *indexFilename = argv[2];
    if (! index_validate(*indexFilename)) { // Validate index filename
      exit(1);
    }

    return 0;
  } else {
    fprintf(stderr,
            "Invalid number of arguments.\nFormat: pageDirectory indexFilename\n");
    exit(1);
  }
}

/*************** searchIndex ****************/
/*
 * Given the pageDirectory and indexFilename, searches through the provided
 * index and outputs any documents that match the query provided through stdin.
 */
static void
searchIndex(char* pageDirectory, char* indexFilename)
{
  char* input = NULL;
  index_t* index = read_index(indexFilename);
  int size = -1; // Keeps track of the number of words in the query
  
  fprintf(stdout, "Query? ");
  while ((input=file_readLine(stdin)) != NULL) { // Reads user-input until EOF
    const int len = strlen(input);
    char* words[len];
    size = readQuery(input, words, len); // Tokenizes input into words array

    // Prints parsed input
    fprintf(stdout, "Query: ");
    for (int i = 0; i < size; i++) {
      fprintf(stdout, "%s ", words[i]);
    }
    fprintf(stdout, "\n");

    // Checks if the input query contains words
    if (size > 0) {
      if (queryCheck(index, words, size)) { // Checks if input line is valid
        counters_t* result = counters_new();
        counters_t* temp = counters_new();

        if (size == 1) { // Checks if the input only contains one word
          ctrsCopy(result, hashtable_find(index, words[0]));
        } else {
          bool streak = false; // True if there is an ongoing "and" sequence

          // Loops through every word from the input
          for (int i = 0; i < size; i++) {
            if (strcmp("or", words[i]) == 0) { // Check if "or" is found
              // Adds the current "and" sequence to the running total
              orProcess(result, temp); 
              counters_delete(temp);
              temp = counters_new();
              streak = false;
            } else { // Runs if the operation is an "and"
              if (strcmp("and", words[i]) != 0) {
                if (streak) { // Checks if a streak has been started
                  // Finds intersect of current "and" sequence and next word
                  counters_t* newAnd = counters_new();
                  ctrsCopy(newAnd, hashtable_find(index, words[i]));
                  andProcess(temp, newAnd);
                  counters_delete(newAnd);
                } else { // Runs if a streak has not been started yet
                  ctrsCopy(temp, hashtable_find(index, words[i]));
                  streak = true;
                }
              }
              // Adds the last and sequence to the ongoing total
              if (i == size - 1) {
                orProcess(result, temp);
              }
            }
          }
        }
        resPrint(result, pageDirectory); // Prints formatted output

        counters_delete(result);
        counters_delete(temp);
      }
    }

    free(input);
    fprintf(stdout, "-----------------------------------------------\n");
    fprintf(stdout, "Query? ");
  }
  index_delete(index);
}

/*************** readQuery **************/
/*
 * Given the input line, an initialized array for words, and a number of slots
 * in the array, tokenizes the input line into individual words stored inside
 * of the provided array and returns the number of words in the array.
 * Inspired by COVID example.
 */
static int
readQuery(char* input, char* words[], const int wordCount)
{
  // Checks if inputs are non-NULL and array size is not less than 1
  if (input == NULL || words == NULL || wordCount < 1) {
    return -1;
  }

  // Loops through the input to look for any invalid characters
  for (int i = 0; i < wordCount; i++) {
    char c = input[i];
    // Checks if the character is not a white space or alpha
    if ((isalpha(c) == 0) && (isspace(c) == 0)) {
      fprintf(stderr,
              "Error: bad character '%c' in query.\n", c);
      return -1;
    } else if (isspace(c) != 0) { // Checks if the character is a white space
      input[i] = ' '; // Convert all white spaces to spaces
    }
  }

  int resIdx = 0; // Keeps track of number of words in input

  char* line = input;
  while (*line != '\0') { // Runs until the end of the line
    if (isalpha(*line) != 0) { // Checks if the string start is an alpha
      words[resIdx++] = normalizeWord(line);
      char* end = strchr(line,  ' '); // Moves character pointer to next space

      if (end == NULL) { // Checks if next space does not exists
        return resIdx;
      } else { // Runs if next space exists
        *end = '\0'; // NULL terminate at that location
        line = end + 1; // Move string start to after the NULL terminator
        // Runs while the string start is a white space
        while (isspace(*line) != 0) {
          line++;
        }
      } 
    } else { // Runs if string start is not an alpha
      // Runs while the string start is a white space
      while (isspace(*line) != 0) {
        line++;
      }
    }
  }
  return resIdx;
}

/************** queryCheck ***************/
/*
 * Given an index, an array of words, and the size of the array, checks if the
 * input word sequence contain illegal syntax and returns a boolean depedent on
 * whether or not the input word sequence is valid.
 */
static bool
queryCheck(index_t* index, char** words, int size)
{
  bool opSeq = false; // True if there is a sequence of operators

  // Checks if "and" or "or" is first word
  if ((strcmp("and", words[0]) == 0)
       || (strcmp("or", words[0]) == 0)) {
    fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
    return false;

  // Checks if "and" or "or" is last word
  } else if ((strcmp("and", words[size-1]) == 0)
              || (strcmp("or", words[size-1]) == 0)) {
    fprintf(stderr, "Error: '%s' cannot be last\n", words[size-1]);
    return false;
  }

  // Checks for double operators
  for (int i = 0; i < size; i++) { // Loops through all words
    // Checks if the word is an "and" or "or"
    if ((strcmp("and", words[i]) == 0) || (strcmp("or", words[i]) == 0)) {
      if (! opSeq) { // Checks if the operator sequence has not started
        opSeq = true;
      } else { // Runs if the operator sequence has started
        fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n",
                words[i-1], words[i]);
        return false;
      }
    } else { // Runs if the word is not an "and" or "or"
      opSeq = false;
    }
  }

  return true;
}

/************** resPrint ***************/
/*
 * Given a counters set containing the result of processing the query and the
 * pageDirectory, prints out the matched documents (if any).
 */
static void
resPrint(counters_t* res, char* pageDirectory)
{
  int slots = ctrsDocsCount(res); // Keeps track of total number of docIDs
  if (slots == 0) { // Checks if there are no documents in the counters set
    fprintf(stdout, "No documents match.\n");
  } else { // Runs if there are documents in the counters set
    docScore_t* docs[slots]; // An array for a struct of docID and count

    // Initializes all values in the array to NULL
    for (int i = 0; i < slots; i++) {
      docs[i] = NULL;
    }

    // Adds all docScore items to the docs array
    counters_iterate(res, docs, resPrintHelper);

    int validDoc = 0; // Keeps track of the number of valid documents
    // Finds number of documents with more than zero for score
    for (int i = 0; i < slots; i++) { // Loops through all documents
      docScore_t* currDoc = docs[i];

      // Checks if the document's score is greater than zero
      if (currDoc->score > 0) {
        validDoc++;
      }
    }

    if (validDoc == 0) { // Checks if there are no valid documents
      fprintf(stdout, "No documents match.\n");
    } else { // Runs if there are more than zero valid documents
      countSort(docs, slots); // Sorts by scores in descending order

      fprintf(stdout, "Matched %d documents:\n", validDoc);

      // Prints out the formatted output for the docID, score, and URL
      for (int i = 0; i < slots; i++) { // Loops through all documents
        docScore_t* currDoc = docs[i];
        // Checks if the document has a score greater than zero
        if (currDoc->score > 0) {
          char* url = pagedir_getURL(pageDirectory, currDoc->docID);
          fprintf(stdout, "score  %d doc  %d: %s\n",
                  currDoc->score, currDoc->docID, url);
          free(url);
        }
      }
    }

    // Frees all docScore items in the docs array
    for (int i = 0; i < slots; i++) { // Loops through all documents
      free(docs[i]);
    }
  }
}

/************** resPrintHelper **************/
/*
 * Supports the main resPrint function by adding documents IDs and their
 * respective counts to an array of docScore type items.
 */
static void
resPrintHelper(void* arg, const int key, const int count)
{
  docScore_t** docs = arg;
  docScore_t* doc = docScore_new(key, count);
  int idx = 0;

  // Identifies the next index available
  while (docs[idx] != NULL) { // Runs while the doc at the index is not NULL
    idx++;
  }
  docs[idx] = doc;
}

/************* countSort ***************/
/*
 * Takes a complete array of pointers for docScore type items and sorts them
 * in descending order according to score.
 */
static void
countSort(docScore_t** docs, int size)
{
  // Does sorting alorithm by going through all elements
  for (int i = 0; i < size-1; i++) {
    docScore_t* currMax = docs[i + 1]; // Initialize element for comparing
    int idx = i + 1; // Keeps track of index of element with highest score
    // Compares currMax to rest of elements in array
    for (int j = i + 1; j < size; j++) {
      // Checks if there are higher scores and replaces currMax if so
      if (currMax->score < docs[j]->score) {
        currMax = docs[j];
        idx = j;
      }
    }
    // Checks if the currMax score is higher than the starting element's score
    if (currMax->score > docs[i]->score) {
      // Creates a temporary docScore_t* for starting element for replacement
      docScore_t* temp = docScore_new(docs[i]->docID, docs[i]->score);
      free(docs[i]); // Get rid of old starting element
      docs[i] = currMax;
      docs[idx] = temp;
    }
  }
}

/************** docScore_new *****************/
/*
 * Given an ID and value for the ID's score, initializes a new docScore type
 * containing those two integers.
 * The user must free() this later.
 */
static docScore_t*
docScore_new(int id, int value)
{
  docScore_t* docScore = malloc(sizeof(docScore_t));

  if (docScore == NULL) {
    return NULL;
  } else {
    docScore->docID = id;
    docScore->score = value;
    return docScore;
  }
}

/************** andProcess ***************/
/*
 * Given a counters set that holds the running total and the new counters set
 * being compared, processes the intersect of the two counters sets and saves
 * it to the running total counters set.
 */
static void
andProcess(counters_t* total, counters_t* new)
{
  ctrsMerge(total, new);
  ctrsMerge(new, total);
  counters_iterate(new, total, andProcessHelper);
}

/************** andProcessHelper ******************/
/*
 * Supports the main andProcess function by finding the minimum value between
 * two counts between the counters sets and sets any non-existing keys with a
 * count of 0 in the first counters set.
 */
static void
andProcessHelper(void* arg, const int key, const int count)
{
  counters_t* ctrs1 = arg;
  int min = count;

  // Checks if the first counters set's count is greater than that of second
  if (counters_get(ctrs1, key) > min) {
    counters_set(ctrs1, key, min);
  }
}

/************* orProcess **************/
/*
 * Given a counters set that holds the running total and the temporary counters
 * set being added, processes the union of the two counters sets and saves it
 * to the running total counters set.
 */
static void
orProcess(counters_t* total, counters_t* new)
{
  counters_iterate(new, total, orProcessHelper);
}

/************** orProcessHelper ****************/
/*
 * Supports the orProcess function by doing the adding for the counts of each
 * key.
 */
static void
orProcessHelper(void* arg, const int key, const int count)
{
  counters_t* ctrs1 = arg;
  int sum = counters_get(ctrs1, key);
  sum += count;
  counters_set(ctrs1, key, sum);
}

/************** ctrsMerge **************/
/*
 * Given two counters sets, irons out differences between the two and adds
 * missing entries in the first counters set with the keys from the second
 * counters set.
 */
static void
ctrsMerge(counters_t* ctrs1, counters_t* ctrs2)
{
  counters_iterate(ctrs2, ctrs1, ctrsMergeHelper);
}

/************** ctrsMergeHelper *******************/
/*
 * Supports the main ctrsMerge function by setting keys in the first counters
 * set equal to zero if the key from the second counters set does not exist
 * in the first counters set.
 */
static void
ctrsMergeHelper(void* arg, const int key, const int count)
{
  counters_t* ctrs1 = arg;

  // Checks if the key does not exists in the first counters set
  if (counters_get(ctrs1, key) == 0) {
    counters_set(ctrs1, key, 0);
  }
}

/************** ctrsCopy *************/
/*
 * Given two counters sets, copies the contents of the second counters set
 * into the first counters set.
 */
static void
ctrsCopy(counters_t* ctrs1, counters_t* ctrs2)
{
  counters_iterate(ctrs2, ctrs1, ctrsCopyHelper);
}

/************** ctrsCopyHelper **************/
/*
 * Supports the ctrsCopy function by setting the keys with their values from
 * the second counters set into the first counters set.
 */
static void
ctrsCopyHelper(void *arg, const int key, const int count)
{
  counters_t* ctrs1 = arg;

  counters_set(ctrs1, key, count);
}

/************** ctrsDocsCount **************/
/*
 * Given a counters set, calculates the total number of keys in the counters
 * set.
 */
static int
ctrsDocsCount(counters_t* ctrs)
{
  int total = 0;
  counters_iterate(ctrs, &total, ctrsDocsCountHelper);
  return total;
}

/************** ctrsDocsCountHelper *****************/
/*
 * Supports the ctrsDocsCount function by adding to the running total for each
 * iteration.
 */
static void
ctrsDocsCountHelper(void* arg, const int key, const int count)
{
  int* total = arg;
  ++*total;
}