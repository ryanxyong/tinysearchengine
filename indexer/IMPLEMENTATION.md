# Ryan Yong
### CS50 Lab 5
### CS50 Spring 2021

## Implementation Spec

This file will cover the implementation of the ideas and aims that are
presented in the requirements and the design spec.

The main topics include:

Data Structures, Control Flow, Other Modules, Function Prototypes,
Error Handling and Recovery, and Testing Plan

## Data Structures

We use three data structures: a 'hashtable' of words and counters, 'counters'
of docID and number of occurences, and an 'index' being the 'hashtable' with
words for keys and 'counters' for items.

## Control Flow
### main

`main` calls `parseArgs` on its arguments and `indexBuild` on the parsed
pageDirectory and indexFilename and returns 0 after successfully running.

### parseArgs

`parseArgs` calls `pagedir_validate` on pageDirectory and `indexdir_init` on
indexFilename and returns 0 if both paths are valid. If not, the function
exits with a non-zero value.

Pseudocode for `parseArgs`:

```
check if the correct number of arguments were inputted
  if so
    validate the page directory and index filename
      if validation fails exit with a non-zero value
    return 0 upon completion
  if not
    print an error message and exit with a non-zero value
```

### indexBuild

`indexBuild` calls `index_new`, `pagedir_load`, `indexPage`, `webpage_delete`,
`index_save`, and `index_delete` to create the index (does not return a value).

Pseudocode for `indexBuild`:

```
create a new index
while a webpage can be loaded
  go through each page and add the words to the index
  delete the webpage
save the index to a file
delete the index
```

### indexPage

`indexPage` calls `webpage_getNextWord`, `normalizeWord`, and `index_add` to add words from a page into the index (does not return a value).

Pseudocode for `indexPage`:

```
only run if the index and page are both not NULL
  while the webpage has another word
    normalize the word and add it to the index
  free the word
```

## Other Modules

### index

The `index` module center-piece is that it is an alias of the hashtable type.

Pseudocode for `index_new`:

```
return a new hashtable containing a set number of slots
```

Pseudocode for `index_add`:

```
check if either the index or word are null
  if not
    return false upon failure
try finding the word in the index
  if not found
    add the word and new counters set containing the information to the index
  if found
    find the counters set for the word and add onto the current count
return true upon completion
```

Pseudocode for `indexdir_init`:

```
allocate memory for the provided index filename
try opening the file path to write
  if not possible
    free the path
    print error message and return false
  if possible
    close the file and free the path
    return true upon completion
```

Pseudocode for `index_validate`:

```
allocate memory for the provided index filename
try opening the file path to read
  if not possible
    free the path
    print error message and return false
  if possible
    close the file and free the path
    return true upon completion
```

Pseudocode for `ctrs_docCount`:

```
print the document ID number and the word occurrences count
```

Pseudocode for `doc_print`:

```
print the word
  iterate through the counters set and print document ID and word count pairs
```

Pseudocode for `index_save`:

```
if able to open the file at the path provided for writing
  iterate through the index and print the contents
  close the file after finishing
```

Pseudocode for `word_delete`:

```
delete each the given counters set
```

Pseudocode for `index_delete`:

```
if the index is not null
  delete the index and its contents
```

Pseudocode for `read_index`:

```
if the given index filename is readable
  make an index with the number of lines as the number of slots
  while there is another word in the given index file
    initialize a new counters set, docID, and count
    while there are more docID and count pairs in the file
      add those docIDs to the counter and set their count values
    insert the counters set into the index
  return the index upon completion
if not readable
  print an error message and return NULL upon failure
```

### pagedir

The psuedocode for `pagedir_init` and `pagedir_save` are unchanged from crawler,
but there are two new functions for indexer.

Pseudocode for `pagedir_validate`:

```
allocate memory for the path name
try opening the file at that path name for reading
  if not found
    print an error and free the path name
    return false upon failure
  if found
    close the file and free the path name
    return true upon completion
```

Pseudocode for `pagedir_load`:

```
determine the number of digits for the current document ID
allocate memory for the path name
try opening the file at that path name for reading
  if not found
    free the path name
    return NULL upon failure
  if found
    close the file and free the path name
    print an error message and return NULL upon failure
```

### word

The `word` module assigns the role of normalizing words to one function by utilizing the ctype module in particular.

Pseudocode for `normalizeWord`:

```
for each character in the string
  make the character lowercase
return 0 upon completion
```

## Function Prototypes
### indexer

Descriptions provided inside of indexer.c.

```c
int main(const int argc, char* argv[]);
static int parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```

### index

Descriptions provided inside of index.h.

```c
index_t* index_new(const int num_slots);
bool index_add(index_t* index, char* word, int docID);
bool indexdir_init(const char* indexFilename);
void ctrs_docCount(void* indexFilename, const int docID, const int count);
void doc_print(void* indexFilename, const char* word, void* ctrs);
void index_save(index_t* index, const char* indexFilename);
void word_delete(void* item);
void index_delete(index_t* index);
index_t* read_index(char* indexFilename);
```

### pagedir

Descriptions provided inside of pagedir.h.

```c
bool pagedir_init(const char* pageDirectory);
bool pagedir_validate(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

### word

Descriptions provided inside of word.h.

```c
int normalizeWord(char* word);
```

## Error Handling and Recovery

Checks for inputs have been enforced throughout the code, so any problems will
be visualized with an error message and non-zero exit status. For instance, the
`parseArgs` function gives a non-zero exit status with an error message if the
user inputs an invalid number of arguments.

Some errors are dealt with internally with boolean, integer, or pointer returns
from some helper functions in the code. For instance, the `pagedir_load`
function will return a NULL pointer if the function fails to find a webpage.

## Testing Plan

Complete regression tests (running the program normally and with valgrind)
along the way with letters test at depths of 0 and 10.

In the later stages, complete a systems test with testing.sh and using
indextest.c to verify that the index creation works as intended.