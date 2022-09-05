# Ryan Yong
### CS50 Lab 6
### CS50 Spring 2021

## querier.c

Does the main work for searching for an inputted query in an index and output
any documents that match along with their scores.

### Functionality

Implemented functionality to meet requirements for the 30 point version.
- As specified under the Grading section:
  - "30 points if your querier also prints the document set in decreasing order by score, thus meeting the full specs."

### Usage

The *querier.c* file contains the following functions:

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

### Implementation

The `parseArgs` method parses the user-inputted arguments into usable
information for the program.

The `searchIndex` method calls on `read_index` (from `index` module),
`file_readLine` (from `file` module) `readQuery`, `queryCheck`, `ctrsCopy`,
`orProcess`, `andProcess`, and `resPrint` to go through the inputted query and
output all relevant documents to the query found in the files produced by
`crawler` and the file produced by `index` module.

The `readQuery` method calls on `isalpha` and `isspace` (from `ctype` module),
`strchr` (from `string` module), and `normalizeWord` (from `word` module) to
go through an inputted line and tokenize all words and store them inside of
an array.

The `queryCheck` method calls on `strcmp` (from `string` module) to go through
the array of words and look for any invalidities in the sequence of words
inputted.

The `resPrint` method calls on `ctrsDocsCount`, `counters_iterate` (from
`counters` module), `countSort`, and `pagedir_getURL` (from `pagedir` module)
to go through the resulting counters set from the query search, convert it into
an array of elements that hold the document ID and respective score, and print
out the matching documents (if any), including document ID, respective score,
and URL, for the query search.

The `resPrintHelper` method supports `resPrint` by creating a new element
containing a document ID and respective count and storing it into the next
available spot in the array of those elements.

The `countSort` method takes the completed array of elements that contain a
document's ID and respective score and sorts the element in descending order
according to the score.

The `docScore_new` method allocates space for the element containing document
ID and respective score and assigns provided values to them.

The `andProcess` method calls on `ctrsMerge` and `counters_iterate` (from
`counters` module) to rid of differences in missing keys between two counters
sets and update the values in the main counters set with the intersect values
of the two.

The `andProcessHelper` method calls on `counters_get` and `counters_set` (from
`counters` module) and supports `andProcess` by setting counts for keys to the
minimum count between the two counters sets.

The `orProcess` method calls on `counters_iterate` (from `counters` module) to
update the values in the main counters set with the union values of the two
counters sets provided.

The `orProcessHelper` method calls on `counters_get` and `counters_set` (from
`counters` module) and supports `orProcess` by setting the main counters set's
current key's count to the current sum.

The `ctrsMerge` method calls on `counters_iterate` (from `counters` module) to
go through a pair of counters sets and add all missing keys in the main
counters set with counts of zero.

The `ctrsMergeHelper` method calls on `counters_get` and `counters_set` (from
`counters` module) and support `ctrsMerge` by adding all keys that cannot be
found in the main counters set from the second counters set.

The `ctrsCopy` method calls on `counters_iterate` (from `counters` module) to
copy the contents of the second counters set to main counters set.

The `ctrsCopyHelper` method calls on `counters_set` (from `counters` module)
and supports `ctrsCopy` by copying all keys and their counts to the new
counters set.

The `ctrsDocsCount` method calls on `counters_iterate` (from `counters` module)
to return the total number of documents inside of a counters set.

The `ctrsDocsCountHelper` method supports `ctrsDocsCount` by incrementing the
running total by one with each iteration through a counters set.

### Assumptions

As listed in the lab requirements:

"`pageDirectory` has files named 1, 2, 3, ..., without gaps", the content of
`pageDirectory` is formatted correctly, and the content of the `indexFilename`
file is formatted correctly.

### Files

`Makefile` - compilation

`querier.c` - program

`testing.sh` - system testing

`testing.out` - result of `make test &> testing.out`

### Compilation

To compile `make`.

### Testing

Testing is done with `testing.sh`, the included `fuzzquery.c`, and various
files inside of the `testFiles` directory (containing queries to test from
`fuzzquery.c` as well as additional edge/invalid cases).

Error messages will be printed above all of the queries because stdout versus
stderr.