/*
 * pagedir.c - 'pagedir' module
 *
 * see pagedir.h for more information
 *
 * Ryan Yong, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"
#include "file.h"

/************** pagedir_init *************/
/* see pagedir.h for description */
bool
pagedir_init(const char* pageDirectory)
{
  char* ext = "/.crawler"; // Character string for pageDirectory extension
  // Allocates memory for path
  char* path = malloc(strlen(pageDirectory)+strlen(ext)+1);
  sprintf(path, "%s%s", pageDirectory, ext); // Writes path
  FILE* fp = NULL;

  if ((fp=fopen(path, "w")) == NULL) { // Checks if file is writeable at path
    fprintf(stderr, "Error: file \"%s\" does not exist.\n", path);
    free(path);
    return false;
  } else { // Runs if file is writeable at path
    fclose(fp);
    free(path);
    return true;
  }
}

/************** pagedir_validate ***************/
/* see pagedir.h for description */
bool
pagedir_validate(const char* pageDirectory)
{
  char* ext = "/.crawler"; // Character string for pageDirectory extension
  // Allocates memory for path
  char* path = malloc(strlen(pageDirectory)+strlen(ext)+1);
  sprintf(path, "%s%s", pageDirectory, ext); // Writes path
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

/************** pagedir_save **************/
/* see pagedir.h for description */
void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  int IDSpace = 0;      // Count of digits of docID
  int tempID = docID;   // Temporary integer to hold docID for calculation
  while (tempID >= 1) { // Calculates total number of digits in docID
    tempID /= 10;
    IDSpace++;
  }

  // Allocates memory for path
  char* path = malloc(strlen(pageDirectory)+IDSpace+2);
  sprintf(path, "%s/%d", pageDirectory, docID); // Writes the path
  FILE* fp = NULL; 

  fp=fopen(path, "w"); // Opens file at path
  fprintf(fp, "%s\n", webpage_getURL(page)); // Saves URL
  fprintf(fp, "%d\n", webpage_getDepth(page)); // Saves depth
  fprintf(fp, "%s\n", webpage_getHTML(page)); // Saves webpage contents
  
  fclose(fp);
  free(path);
}

/************** pagedir_load ****************/
/* see pagedir.h for description */
webpage_t*
pagedir_load(const char* pageDirectory, const int docID)
{
  int IDSpace = 0;      // Count of digits of docID
  int tempID = docID;   // Temporary integer to hold docID for calculation
  while (tempID >= 1) { // Calculates total number of digits in docID
    tempID /= 10;
    IDSpace++;
  }

  char* path = malloc(strlen(pageDirectory)+IDSpace+2);
  sprintf(path, "%s/%d", pageDirectory, docID); // Writes the path
  FILE* fp = NULL; 

  // Checks if the file is not readable for the docID in the pageDirectory
  if((fp=fopen(path, "r")) == NULL) {
    free(path);
    return NULL;
  } else {
    char* url = file_readLine(fp); // Extracts the URL in the file
    webpage_t* page = webpage_new(url, 0, NULL);
    if (webpage_fetch(page)) { // Gets the HTML for URL
      fclose(fp);
      free(path);
      return page;
    } else { // Runs if unable to get the HTML for URL
      fclose(fp);
      free(path);
      fprintf(stderr, "Error: webpage not found.\n");
      return NULL;
    }
  }
  return NULL;
}

/************** pagedir_getURL ****************/
/* see pagedir.h for description */
char*
pagedir_getURL(const char* pageDirectory, int docID)
{
  int IDSpace = 0;      // Count of digits of docID
  int tempID = docID;   // Temporary integer to hold docID for calculation
  while (tempID >= 1) { // Calculates total number of digits in docID
    tempID /= 10;
    IDSpace++;
  }

  char* path = malloc(strlen(pageDirectory)+IDSpace+2);
  sprintf(path, "%s/%d", pageDirectory, docID); // Writes the path
  FILE* fp = NULL;

  // Checks if the file is not readable for the docID in the pageDirectory
  if((fp=fopen(path, "r")) == NULL) {
    fclose(fp);
    free(path);
    return NULL;
  } else {
    char* url = file_readLine(fp); // Extracts the URL in the file
    fclose(fp);
    free(path);
    return url;
  }
}