# Ryan Yong
### CS50 Lab 6
### CS50 Spring 2021

## Design Spec

Formulated as according to the requirements spec.

TSE Querier is a standalone program that reads the outputted document files
from TSE Crawler and the outputted index file from TSE Indexer to let the
user search for files.

## User Interface

The querier is started up with a command-line with two arguments.

`querier pageDirectory indexFilename`

Afterwards, the user may input queries to search for in the index by 
typing/piping input into stdin.

This will continue until the user inputs EOF.

## Inputs and Outputs

Input: Reads files from the provided directory (pageDirectory) and reads an
index file (indexFilename) to retrieve matching documents for a query.

Output: Gives users documents that match the inputted query.

## Functional Decomposition Into Modules

Modules or functions as follows:

- _main_, which parses arguments and initializes the query searcher.
- _parseArgs_, which does the parsing of arguments provided.
- _searchIndex_, which creates a new index from the index file, reads
user-input, creates an array of words, creates a counters set of the
documents relevant for a query, and prints out any matches.
- _readQuery_, which returns the total number of words in the input after
tokenizing all of them into an array.
- _queryCheck_, which returns a boolean depending on whether or not the input
is free of invalid/illegal sequence of operators.
- _resPrint_, which creates an array of elements containing document ID and
respective score and prints out the information for documents with a non-zero
score.
- _resPrintHelper_, which supports _resPrint_ and adds the new elements in
the next available spot in the array.
- _countSort_, which rearranges items in the array of elements containing
document IDs and scores in descending order based on score.
- _docScore_new_, which creates a new element containing document ID and
respective score.
- _andProcess_, which irons out all differences between two counters sets and
saves the intersect of two counters sets into the main one.
- _andProcessHelper_, which supports _andProcess_ and compares counts of two
keys.
- _orProcess_, which takes two counters sets and saves the union of two
counters sets into the main one.
- _orProcessHelper_, which supports _orProcess_ and calculates the sum of two
document IDs' scores and saves it to the main counters set.
- _ctrsMerge_, which saves keys from the second counters set not in the main
counters set with a score of zero.
- _ctrsMergeHelper_, which supports _ctrsMerge_ and does the adding of missing
keys into the main counters set.
- _ctrsCopy_, which copies contents of one counters set into another one.
- _ctrsCopyHelper_, which supports _ctrsCopy_ and adds all keys with respective
counts into the new counters set.
- _ctrsDocsCount_, which returns the total number of documents in a counters
set.
- _ctrsDocsCountHelper_, which supports _ctrsDocsCount_ and increments the
running total by one with every iteration of the counters set.

## Pseudocode for logic/algorithmic flow

Pseudocode for `querier`:

```
parse the provided arguments, validate the inputs
call searchIndex, with readQuery
```

Pseudocode for `searchIndex`:

```
create a new input string
create a new index with the index file
loop for input until EOF
  create a new array of strings
  tokenize the inputted line for into array
    if illegal character found
      print error statement
      do nothing else and wait for next input
  if query contains more than zero words
    do the search
      create a running total counters set
      create a temporary counters set
      validate the sequence of operators
      if there is only one word
        set the result counters set to that of the word
      else
        initialize boolean for seeing if "and" sequence exists to false
        loop through the array of words
          if the current word is "or"
            find the union of the temporary and running total counters sets
            reset temporary counters
            reset "and" sequence boolean
          else
            if implicit "and" found
              find the intersection of the word's and temporary counters sets
            else
              set temporary counters set to current word's counter set
              set "and" sequence boolean to true
            if on the last "and" sequence
              find the union of the temporary and running total counters sets
      print out matching documents in the now-final running total counters set
```

Pseudocode for `readQuery`:

```
if any parameter is null and size of input is less than one
  return non-zero exit status

loop through each character in input
  if character is not an alpha or a white space
    print error message
    return non-zero exit status
  else if character is a white space
    convert the character to a normal space

initialize an integer to track number of words

loop until end of the line
  if character is an alpha
    add word to words array
    increment number of words count
    look for next space

    if no more spaces
      return number of words
    else
      move to start of next word
      loop until through any and all extra white spaces
        move through input
  else
    loop until through any and all extra white spaces
      move through input
return number of words
```

### queryCheck

Pseudocode for `queryCheck`:

```
initialize boolean for seeing if previous word was an operator
  if the first word is "and" or "or"
    print error message
    return false
  else if the last word is "and" or "or"
    print error message
    return false

loop through all words
  if the word is "and" or "or"
    if the previous operator boolean is false
      set it to true
    else
      print error message
      return false
  else
    set previous operator boolean to false
return true
```

### resPrint

Pseudocode for `resPrint`:

```
initialize integer to hold number of documents in counters set
if there are no documents
  print no match message
else
  declare array of elements containing document ID and score
  initialize all elements in array
  iterate through counters set of document ID and score
    add documents and scores into array
  initialize integer to track number of documents with valid score
  loop through array of document ID and score pairs
    if current document has score greater than zero
      increment number of documents integer
  if no documents have score greater than zero
    print no match message
  else
    sort all items in the array in descending order by score
    print out matched number of documents with score greater than zero
    loop through array of document ID and score pairs
      if current document has score greater than zero
        print out its information
```

### resPrintHelper

Pseudocode for `resPrintHelper`:

```
initialize a new element containing the current document ID and score
initialize an integer to track next available index

loop through array of document ID and score pairs until NULL
  increment next available index tracker
set current document ID and score element at available index
```

### countSort

Pseudocode for `countSort`:

```
loop through array of document ID and score pairs
  initialize a temporary document ID and score pair to next pair for maximum
  initialize maximum index (may not be maximum at this point)
  loop through rest of array after current document ID and score pair
    if temporary pair's score less than rest of array pair's score
      replace pair in temporary with rest of array pair
  if the temporary pair's score greater than current pair
    switch places for current pair and temporary pair
```

### docScore_new

Pseudocode for `docScore_new`:

```
if unable to create document ID and score pair
  return NULL value
else
  set document ID to provided
  set score to provided
  return pair
```

### andProcess

Pseudocode for `andProcess`:

```
merge counters sets
iterate through second counters set
  set counts in main counters set to minimum between two counts
```

### andProcessHelper

Pseudocode for `andProcessHelper`:

```
initialize integer for minimum count
if the count for key in main counters set is greater than minimum
  set its value to minimum
```

### orProcess

Pseudocode for `orProcess`:

```
iterate through second counters set
  set counts in main counters to sum between two counts
```

### orProcessHelper

Pseudocode for `orProcessHelper`:

```
initialize integer for count sum
set sum to addition of counts for both counters sets
set count in main counters set to sum
```

### ctrsMerge

Pseudocode for `ctrsMerge`:

```
iterate through seconds counters set
  add keys from second counters set not in main counters set with count of zero
```

### ctrsMergeHelper

Pseudocode for `ctrsMergeHelper`:

```
if key not in main counters set
  add key into main counters set with count of zero
```

### ctrsCopy

Pseudocode for `ctrsCopy`:

```
iterate through second counters set
  copy contents into main counters set
```

### ctrsCopyHelper

Pseudocode for `ctrsCopyHelper`:

```
add key and count into main counters set
```

### ctrsDocsCount

Pseudocode for `ctrsDocsCount`:

```
initialize integer for running total
iterate through counters set
  increment running total
```

### ctrsDocsCountHelper

Pseudocode for `ctrsDocsCountHelper`:

```
increment running total
```

## Major Data Structures

The data structures of importance here are the array of strings, counters sets,
and an element containing and document ID and score pair.

## Testing Plan

Doing regression tests along the way by compiling the program and using
pre-existing page directories and index files.

Doing systems testing by running through boundary cases (invalid number of
arguments, correct number of arguments but some invalid arguments) and
various inputs created by `fuzzquery.c` and some manually generated containing
invalid characters and extra white spaces or capitalization irregularities.

Checking for memory leaks with valgrind.