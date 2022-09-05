#!/bin/bash
#
# testing.sh - tests various cases for querier
#
# Ryan Yong, May 2021
#

sharedOut="/thayerfs/home/f004gx3/cs50-dev/shared/tse/output"
check="valgrind --leak-check=full --show-leak-kinds=all"

# Test cases for argument bounds
# No arguments provided
./querier < testFiles/fuzzTestQueriesLetters10

# One argument provided
./querier $sharedOut/letters-10 < testFiles/fuzzTestQueriesLetters10

# Two arguments provided but invalid pageDirectory
./querier adsdfssadfdafdsaf $sharedOut/letters-10.index < testFiles/fuzzTestQueriesLetters10

# Two arguments provided by both invalid
./querier asdffdsasdfasdfa sdafsfdadfasfda < testFiles/fuzzTestQueriesLetters10

# Three argument provided
./querier $sharedOut/letters-10 $sharedOut/letters-10.index $sharedOut/letters-0 < testFiles/fuzzTestQueriesLetters10


# Test cases with valid arguments and invalid queries
# letters testing with depth 10
./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/invalidTestQueries

# wikipedia testing with depth 1
./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/invalidTestQueries

# Test cases with valgrind with valid arguments and invalid queries
# letters testing with depth 10
$check ./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/invalidTestQueries

# wikipedia testing with depth 1
$check ./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/invalidTestQueries


# Test cases with valid arguments and valid queries (with extra white spaces or capitalizations)
# letters testing with depth 10
./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/formattingTestQueries

# wikipedia testing with depth 1
./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/formattingTestQueries

# Test cases with valgrind with valid arguments and valid queries (with extra white spaces or capitalizations)
# letters testing with depth 10
$check ./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/formattingTestQueries

# wikipedia testing with depth 1
$check ./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/formattingTestQueries


# Test cases with valid arguments and valid queries
# letters testing with depth 10
./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/fuzzTestQueriesLetters10

# wikipedia testing with depth 1
./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/fuzzTestQueriesWikipedia1

# Test cases with valgrind with valid arguments and valid queries
# letters testing with depth 10
$check ./querier $sharedOut/letters-10 $sharedOut/letters-10.index < testFiles/fuzzTestQueriesLetters10

# wikipedia testing with depth 1
$check ./querier $sharedOut/wikipedia-1 $sharedOut/wikipedia-1.index < testFiles/fuzzTestQueriesWikipedia1