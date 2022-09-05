/*
 * crawler.c - crawls through webpages and saves visited webpages
 *
 * usage - User must provide the seed URL, page directory,
 * and max depth to crawl through as arguments
 *
 * Ryan Yong, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "bag.h"
#include "webpage.h"
#include "pagedir.h"

// prototype functions
static int parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);

/************** main ****************/
int
main(const int argc, char* argv[])
{
  char* seedURL = NULL;       // Initializes seedURL character string
  char* pageDirectory = NULL; // Initializes pageDirectory character string
  int maxDepth = 0;           // Initializes maxDepth integer

  // Checks if arguments are parsed correctly
  if (parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth) == 1) {
    return 1;
  }
  
  crawl(seedURL, pageDirectory, maxDepth);

  return 0;
}

/*************** parseArgs ****************/
/*
 * Given user-inputted arguments, assigns them to variables and then returns
 * the exit status.
 */
static int
parseArgs(const int argc, char* argv[],
          char** seedURL, char** pageDirectory, int* maxDepth)
{
  if (argc == 4) { // Checks if correct number of arguments are supplied
    *seedURL = argv[1]; // Sets seedURL equal to user-input
    char* norm = normalizeURL(*seedURL); // Normalizes seedURL
    if (! isInternalURL(norm)) { // Checks if seedURL is internal
      free(norm);
      fprintf(stderr, "Non-internal URL.\n");
      return 1;
    }
    free(norm);
    *pageDirectory = argv[2]; // Sets pageDirectory equal to user-input
    
    if (! pagedir_init(*pageDirectory)) { // Checks if the directory is valid
      fprintf(stderr, "Invalid path directory.\n");
      return 1;
    }

    // Initializes a temporary string to hold maxDepth in string form
    const char* maxDepthString = argv[3];
    *maxDepth = 0;
    char excess; // Keeps track of excess characters
    // Converts character strings to integers
    sscanf(maxDepthString, "%d%c", maxDepth, &excess);
    if (*maxDepth > 10 || *maxDepth < 0) {
      fprintf(stderr,
              "Depth %d out of bounds. Enter value between 0 and 10.\n",
              *maxDepth);
      return 1;
    }

    return 0;
  } else { // Runs if incorrect number of arguments are supplied
    fprintf(stderr, 
            "Provided %d arguments but need 3\nFormat: %s seedURL pageDirectory maxDepth.\n",
            argc-1, argv[0]);
    return 1;
  }
}

/***************** crawl *****************/
/*
 * Given seedURL, pageDirectory, and maxDepth, crawls through the web
 * and saves webpages.
 */
static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  // Allocates memory for a temporary copy of seedURL
  char* seedURLCopy = malloc(strlen(seedURL)+1);
  strcpy(seedURLCopy, seedURL); // Copies seedURL to the temporary copy
  // Initializes a new webpage with the seedURL copy at depth 0
  webpage_t* wp = webpage_new(seedURLCopy, 0, NULL);

  // Initializes a new hashtable with an arbitrary size of 100
  hashtable_t* ht = hashtable_new(100);
  hashtable_insert(ht, seedURL, ""); // Inserts the seedURL into the hashtable

  bag_t* bag = bag_new(); // Initializes a new bag
  bag_insert(bag, wp); // Inserts the webpage into the bag

  int docID = 1; // Provides the ID number for documents written
  webpage_t* page = NULL;

  // Runs while the page extracted from the bag is not null
  while ((page=bag_extract(bag)) != NULL) {
    if (webpage_fetch(page)) { // Checks if the page has a fetchable HTML
      logr("Fetched", webpage_getDepth(page), webpage_getURL(page));

      pagedir_save(page, pageDirectory, docID++); // Saves the webpage

      if (webpage_getDepth(page) < maxDepth) { // Checks if at max depth
        logr("Scanning", webpage_getDepth(page), webpage_getURL(page));
        pageScan(page, bag, ht); // Scans for more webpages
      }
    }

    webpage_delete(page);
  }
  hashtable_delete(ht, NULL);
  bag_delete(bag, webpage_delete);
}

/****************** pageScan **************/
/*
 * Scans a webpage for URLs to add to the bag and hashtable as necessary.
 */
static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  char* next = NULL; // Character string for next URL
  int idx = 0; // Index of URL

  // Runs while the next webpage URL is not null
  while ((next=webpage_getNextURL(page, &idx)) != NULL) {
    logr("Found", webpage_getDepth(page), next);
    char* norm = normalizeURL(next); // Normalizes URL in temporary string
    if (isInternalURL(norm)) { // Checks if the URL is internal
      // Checks if the URL is successfully added to hashtable
      if (hashtable_insert(pagesSeen, norm, "")) {
        // Initializes a new website for seen with 1 greater depth
        logr("Added", webpage_getDepth(page), next);
        webpage_t* seen = webpage_new(norm, webpage_getDepth(page)+1, NULL);
        bag_insert(pagesToCrawl, seen);
      } else {
        logr("IgnDupl", webpage_getDepth(page), next);
        free(norm); // Freed if URL is not successfully added
      }
    } else {
      logr("IgnExtrn", webpage_getDepth(page), next);
      free(norm); // Freed if URL is not internal
    }
    free(next);    
  }
}

/**************** logr *****************/
/*
 * Provided in the CS50 Lecture Notes
 * Allows for logging of program.
 */
static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}
