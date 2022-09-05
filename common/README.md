# Ryan Yong
### CS50 Lab 4
### CS50 Spring 2021

## pagedir

Does the main work for initializing a defined page directory and saving
webpages as called for in crawler.c.

### Usage

The *pagedir* module, defined in `pagedir.h` and implemented in `pagedir.c` and
exports the following functions:

```c
bool pagedir_init(const char* pageDirectory);
bool pagedir_validate(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

### Implementation

The `pagedir_init` method initializes and checks the validity of the provided
page directory.

The `pagedir_validate` method checks the validity of reading from the provided
page directory.

The `pagedir_save` method saves a webpage to a file as called in crawler.c.

The `pagedir_load` method fetches and returns a webpage from a document in a
page directory.

### Assumptions

As listed in the lab requirements:
"The pageDirectory does not contain any files whose name is an integer
(i.e., 1, 2, ...)"

### Files

`Makefile` - compilation

`pagedir.h` - interface

`pagedir.c` - implementation

### Compilation

To compile `make`.

### Testing

Testing is included in the systems test with the testing.sh file in crawler
and indexer.


## index

Does the main work for validating a defined page directory, keeping track of an
index of words, and saving the index to a file in indexer.c.

### Usage

The *index* module, defined in `index.h` and implemented in `index.c` and
exports the following functions:

```c
index_t* index_new(const int num_slots);
bool index_add(index_t* index, char* word, int docID);
bool indexdir_init(const char* indexFilename);
bool index_validate(const char* indexFilename);
void ctrs_docCount(void* indexFilename, const int docID, const int count);
void doc_print(void* indexFilename, const char* word, void* ctrs);
void index_save(index_t* index, const char* indexFilename);
void word_delete(void* item);
void index_delete(index_t* index);
index_t* read_index(char* indexFilename);
```

### Implementation

The `index_new` method creates a new index by implementing a hashtable type
data structure.

The `index_add` method adds entries into the index (hashtable) with the counts
of the word's occurrences in each document.

The `indexdir_init` method creates the output file for the index.

The `index_validate` method works almost identically to `indexdir_init` except
it checks if the file is readable rather than writeable.

The `ctrs_docCount` method assists `doc_print` with printing a document's ID
and word occurrence count.

The `doc_print` method assists `index_save` with printing the word at a 
position in the index.

The `index_save` method saves the index to a given output file.

The `word_delete` method assists `index_delete` with deleting the counters
for a word.

The `index_delete` method deletes the entire index.

The `read_index` method loads an index file, creates a new index data structure
with the information, and outputs the new index into a new index file.

### Assumptions

None

### Files

`Makefile` - compilation

`index.h` - interface

`index.c` - implementation

### Compilation

To compile `make`.

### Testing

Testing is included in the systems test with the testing.sh and indextest.c
files in indexer.


## word

Makes words inputted normal, which means to convert it to all lowercase.

### Usage

The *word* module, defined in `word.h` and implemented in `word.c` and
exports the following functions:

```c
int normalizeWord(char* word);
```

### Implementation

The `normalizeWord` method takes a character string and converts each
character into lowercase.

### Assumptions

None

### Files

`Makefile` - compilation

`word.h` - interface

`word.c` - implementation

### Compilation

To compile `make`.

### Testing

Testing is included in the systems test with the testing.sh file in indexer.