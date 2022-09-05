# Ryan Yong
### CS50 Lab 5
### CS50 Spring 2021

## indexer.c

Does the main work for building an index of all words (with three or more
characters) and the number of times they occur in each document.

### Usage

The *indexer.c* file contains the following functions:

```c
static int parseArgs(const int argc, char* argv[],
                     char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(index_t* index, webpage_t* page, const int docID);
```

### Implementation

The `parseArgs` method parses the user-inputted arguments into usable
information for the program.

The `indexBuild` method calls on indexPage, index_save, and index_delete
(from the `index` module) to go through the files produced by crawler and
save the words and their counts in each document to a file.

The `indexPage` method assists `indexBuild` and goes through a file produced by
crawler and adds the words or increments the counts as necessary.

### Assumptions

As listed in the lab requirements:
"pageDirectory has files named 1, 2, 3, ..., without gaps" and the content of
pageDirectory is formatted correctly.

### Files

`Makefile` - compilation

`indexer.c` - program

`testing.sh` - system testing

`testing.out` - result of `make test &> testing.out`

### Compilation

To compile `make`.

### Testing

Testing is done with testing.sh, indextest.c, and the provided indexcmp
program.
For simplicity, `make test` can be used for testing.sh.


## indextest.c

Takes an old index, then makes a new index data structure, and finally creates
a new index file with the new index.

### Usage

The *indextest.c* files contains the following functions:

```c
static int parseArgs(const int argc, char* argv[],
                     char** oldIndexFilename, char** newIndexFilename);
static void make_index(char* oldIndexFilename, char* newIndexFilename);
```

### Implementation

The `parseArgs` method parses the user-inputted arguments into usable
information for the program.

The `make_index` method saves the output of the newly-created index data
structure from the given old index filename into a new index filename.

### Assumptions

As listed in the lab requirements:
The contents of the index file are formatted correctly.

### Files

`Makefile` - compilation

### Compilation

To compile `make`.

### Testing

This file is used for testing.