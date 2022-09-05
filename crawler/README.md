# Ryan Yong
## CS50 Lab 4
## CS50 Spring 2021

### crawler.c

Does the main work for crawling through the web to save webpages up to a
defined depth.

### Usage

The *crawler.c* file exports the following functions:

```c
static int parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);
```

### Implementation

The `parseArgs` method parses the user-inputted arguments into usable
information for the program.

The `crawl` method calls on logr, pageScan, and pagedir_save (from the 
`pagedir` module) to go through the web and save webpages.

The `pageScan` method scans through webpages for URLs to add to the search bag.

The `logr` method is called through crawler.c to log the status of the program.

### Assumptions

As listed in the lab requirements:
"The pageDirectory does not contain any files whose name is an integer
(i.e., 1, 2, ...)"

### Files

`Makefile` - compilation
`crawler.c` - program
`testing.sh` - system testing
`testing.out` - result of `make test &> testing.out`
`valgrind.sh` - valgrind testing
`valTesting.out` - result of `make valgrind &> valTesting.out`

### Compilation

To compile `make`.

### Testing

Testing is done with testing.sh and valgrind.sh (for memory leak checks).
For simplicity, `make test` and `make valgrind` can be used.