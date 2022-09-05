# Ryan Yong
### CS50 Lab 6
### CS50 Spring 2021

## Implementation Spec

This file will cover the implementation of the ideas and aims that are
presented in the requirements and the design spec.

The main topics include:

Data Structures, Control Flow, Other Modules, Function Prototypes,
Error Handling and Recovery, and Testing Plan

## Data Structures

We use three data structures: a double pointer of characters (array of
strings), a counters set, and a struct containing integers for a document ID
and its score.

## Control Flow
### main

`main` calls `parseArgs` on its arguments and `searchIndex` on the parsed
pageDirectory and indexFilename, then exits zero after successfully running.

### parseArgs

`parseArgs` calls `pagedir_validate` on pageDirectory and `index_validate` on
indexFilename and returns 0 if both paths are valid. If not, the function exits
with a non-zero value.

Pseudocode for `parseArgs`:

```
if correct number of arguments provided
  set values for page directory and index filename with arguments
  validate the page directory and index filename
    if validation for either fails
      exit with non-zero value upon failure
  return zero upon completion
else
  print error message
  exit with non-zero value upon failure
```

### searchIndex

`searchIndex` calls `read_index` on indexFilename, `file_readLine` on stdin,
`readQuery` on the stdin input line, words array, and input line length,
`queryCheck` on the index, words array, and number of words, `ctrsCopy` on the
result, temporary, or found counters sets, `hashtable_find` with the index and
words in the words array, `strcmp` on the words in the words array along with
"and" or "or, `counters_new`, `orProcess` with the result and temporary
counters sets, `counters_delete` (eventually) to all counters sets,
`andProcess` on result and word's counter sets, `resPrint` the result counters
set and pageDirectory, and `index_delete` on the index (does not return a
value).

Pseudocode for `searchIndex`:

```
create a new index by reading index file
while input is not EOF, prompt user for input
  initialize a new array of words with slots equal to the input string length
  tokenize all words in the input into the words array
  determine number of words in the words array
  loop through words array
    print out parsed version of words in input line
  if more than zero words in words array
    if input operators in input line not illegal/invalid
      create result counters set
      create temporary counters set
      if only one word in input line
        copy counters set of word into result counters set
      else
        initialize boolean to track if ongoing "and" sequence exists
        loop through every word from the input
          if string comparison of "or" and current word says same
            do the union process for the result and temporary counters sets
            free the temporary counters set
            initialize the temporary counters set
            set the "and" sequence boolean to false
          else
            if string comparison of "and" and current word says different
              if an "and" sequence exists
                initialize a new counters set
                copy the counters set for the current word into the new one
                do the intersect process for the temporary and new counters sets
                free the new counters set (for the current word)
              else
                copy the counters set for the word into the temporary
                set the "and" sequence boolean to true
            if at the last word
              do the union process for the result and temporary counters sets
      print out all of the matching documents with correct formatting
      free result counters set
      free temporary counters set
  free input line
free index
```

### readQuery

`readQuery` calls `isalpha` and `isspace` on the current character in the input
line, `normalizeWord` on the start of a new word, and `strchr` on the remaining
input line and space character and returns the number of words in the input
line.

Pseudocode for `readQuery`:

```
if input or words array pointers are NULL or the input length is less than one
  return non-zero exit status upon failure
loop through every character in the input pointer
  if a character is not an alpha and not a white space
    print error message
    return non-zero exit status upon failure
  else if character is a white space
    convert the character at those index to a normal space
initialize number of words count (start at 0)
while the character at the start of a pointer is not the end
  if the starting character is an alpha
    add the normalized version of the word pointer to the words array
    increment number of words
    find the next occurrence of a space and save it to a pointer
    if the next occurrence of the space was not NULL
      return number of words
    else
      NULL terminate the space
      move the line start pointer up one
      while the pointer is a white space
        move the line start pointer up one
  else
    while the line start pointer is a white space
      move the line start pointer up one
return the number of words
```

### queryCheck

`queryCheck` calls `strcmp` on words in the words array as well as "and" or
"or" and returns a boolean for whether or not a query contains invalid
sequences of "and" or "or" operators.

Pseudocode for `queryCheck`:

```
create a boolean for tracking if a sequence of operators has started
if string comparison of "and" or "or" and the first word is same
  print error message
  return false upon failure
else if string comparison of "and" or "or and the last word is same
  print error message
  return false upon failure
loop through every word in the words array
  if string comparison of "and" or "or" of current word is same
    if sequence of operators boolean is false
      set it to true
    else
      print error message
      return false upon failure
  else
    set sequence of oeprators boolean to false
return true upon completion
```

### resPrint

`resPrint` calls `ctrsDocsCount` on the result counters set, `counters_iterate`
on the result counters set, array of docScore elements, and a helper function,
`countSort` on the array of docScore elements, and `pagedir_getURL` on the
pageDirectory and document IDs for items in the array of docScores elements
(does not return a value).

Pseudocode for `resPrint`:

```
initialize integer containing number of documents in the result counters set
if number of documents is zero
  print no match
else
  initialize array of size number of documents for docScore pointer items
  initialize all slots in the docScores array to NULL
  iterate through result counters set and store all document ID and score pairs
    into docScores array
  create integer to track valid number of documents with score over zero
  loop through docScores array
    if current document's score is over zero
      increment valid number of documents
  if zero valid number of documents
    print no match
  else
    sort the docScores array by score in descending order
    print valid number of documents matched
    loop through docScores array
      if current document's score is above zero
        get its URL
        print out its information (ID, score, URL)
        free the URL
  loop through docScores array
    free all document ID and score pairs

```

### resPrintHelper

`resPrintHelper` calls `docScore_new` on the current key and count (does not
return a value).

Pseudocode for `resPrintHelper`:

```
cast the provided argument as an array of docScore item pointers
create a new docScore pointer containing the current key and count
initialize an integer to hold next available index number
while the docScores array is not NULL
  increment number tracking next available index
add the new docScore pointer at the next available index
```

### countSort

`countSort` calls `docScore_new` on a specified document ID and score pair
when replacement takes place (does not return a value).

Pseudocode for `countSort`:

```
loop through docScores array (excluding last item)
  create temporary pointer to docScore item above current one
  loop through all docScore items after current one
    if temporary pointer's score is less than anything in rest
      replace temporary pointer with pointer to larger docScore item
  if the temporary pointer's score is greater than that of the current one
    create a docScore pointer to hold a copy of the current one
    free the current docScore pointer
    add the temporary pointer at the current index
    add the copy into the index of the temporary's old index
```

### docScore_new

`docScore_new` returns a pointer for a new docScore type.

Pseudocode for `docScore_new`:

```
allocate space for the docScore type

if docScore pointer is NULL
  return NULL
else
  assign docID and score with provided values
  return the docScore pointer
```

### andProcess

`andProcess` calls `ctrsMerge` on the two counters sets provided and
`counters_iterate` on the two counters sets and a helper function (does not
return a value).

Pseudocode for `andProcess`:

```
merge the counters sets of the total and the new
  do so in both directions
iterate over the new counters set and pass the total counters set as argument
  find interset of both counters sets
```

### andProcessHelper

`andProcessHelper` calls `counters_get` on the main counters set and the
current key and `counters_set` on the main counters set, current key, and
minimum value (does not return a value).

Pseudocode for `andProcessHelper`:

```
cast the provided argument as a counters set
initialize an integer for minimum with the count for key in second counters set
if count of key in main counters set is greater than the minimum
  set the count of key in main counters to the minimum
```

### orProcess

`orProcess` calls `counters_iterate` on the two counters sets provided and a
helper function (does not return a value).

Pseudocode for `orProcess`:

```
iterate through the new counters set and pass total counters set as argument
  find union of the two counters sets
```

### orProcessHelper

`orProcessHelper` calls `counters_get` on the main counters set and current key
and `counters_set` on the main counters set, current key, and sum value (does
not return a value).

Pseudocode for `orProcessHelper`:

```
cast the provided argument as a counters set
initialize an integer with count for key in main counters set
add the current count to the previous integer to get sum
set the count for key in main counters set to sum
```

### ctrsMerge

`ctrsMerge` calls `counters_iterate` on the two counters sets provided and a
helper function (does not return a value).

Pseudocode for `ctrsMerge`:

```
iterate through the second counters set and pass main counters set as argument
  find missing keys in main counters set and add
```

### ctrsMergeHelper

`ctrsMergeHelper` calls `counters_get` on the main counters set and the current
key and `counters_set` on the main counters set, current key, and a value of
zero (does not return a value).

Pseudocode for `ctrsMergeHelper`:

```
cast the provided argument as a counters set
if the key does not exist in the main counters set
  add the key into the main counters set with a value of zero
```

### ctrsCopy

`ctrsCopy` calls `counters_iterate` on the two counters sets provided and a
helper function (does not return a value).

Pseudocode for `ctrsCopy`:

```
iterate through the second counters set and pass main counters set as argument
  copy entries in second counters set into main counters set
```

### ctrsCopyHelper

`ctrsCopyHelper` calls `counters_set` on the main counters set, current key,
and current count (does not return a value).

Pseudocode for `ctrsCopyHelper`:

```
cast the provided argument as a counters set
add the current key and count into the main counters set
```

### ctrsDocsCount

`ctrsDocsCount` calls `counters_iterate` on the provided counters set, pointer
to a running total, and a helper function and returns the total at the end.

Pseudocode for `ctrsDocsCount`:

```
initialize an integer for a running total
iterate through counters set and pass pointer to the running total as argument
  increment the running total
return the total upon completion
```

### ctrsDocsCountHelper

`ctrsDocsCountHelper` increments the value of the provided pointer to a running
total (does not return a value).

Pseudocode for `ctrsDocsCountHelper`:

```
cast the provided argument as a pointer to an integer
increment the value of the dereferenced pointer
```

## Other Modules

### pagedir

The pseudocode remains unchanged from indexer and crawler, but there is a new
function for querier called `pagedir_getURL` to simplify the process of
getting a URL given a pageDirectory and document ID.

Pseudocode for `pagedir_getURL`:

```
initialize an integer to track number of digits
initialize an integer with a copy of the document ID
while the copy is greater than or equal to zero
  divide the number by ten
  increment the digit count
allocate memory for the path to the file in pageDirectory
create the pathname
if the file at the pathname is not readable
  close the file
  free the pathname
  return NULL
else
  read the first line (contains the URL)
  close the file
  free the pathname
  return the URL
```

## Function Prototypes
### querier

Descriptions provided inside of querier.c.

```c
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
```

## Error Handling and Recovery

Checks for inputs done throughout the code, so problems will be visualized
with an error message and non-zero exit status.

Some errors are dealt with internally with boolean, integer, or pointer returns
from some functions in the code.

## Testing Plan

Completed regression tests (running the program normally and with valgrind)
along the way with wikipedia test at depths of 0 and 1.

Systems test done with `testing.sh`, `fuzzquery.c`, and files inside of the
`testFiles` (all contain 10 queries and are derived from `fuzzquery.c` except
for `invalidTestQueries` and `formattingTestQueries` files) directory to verify
that querier works as intended.