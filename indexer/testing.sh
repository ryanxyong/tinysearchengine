#!/bin/bash
#
# testing.sh - tests various cases for indexer
#
# Ryan Yong, May 2021
#

mkdir testOut
sharedOut="/thayerfs/home/f004gx3/cs50-dev/shared/tse"
check="valgrind --leak-check=full --show-leak-kinds=all"

# Test cases for argument bounds
# No arguments provided
./indexer

# One argument provided
./indexer $sharedOut/output/letters-0

# Two arguments provided but invalid pageDirectory
./indexer asdfsadfasdfafsdfdsaafds testOut/twoArgIPD.index

# Two arguments provided but invalid indexFilename
./indexer $sharedOut/output/letters-0 asdfasdfasdfasdfasf/twoArgIIF.index

# Two arguments provided but both invalid
./indexer asdfasdfasdfasdfasdf asdfasdfasdfasdasdf/twoArgN.index

# Three arguments provided
./indexer $sharedOut/output/letters-0 testOut/letters-0.index $sharedOut

# Testing cases with valid arguments
# letters testing with depth 0
./indexer $sharedOut/output/letters-0 testOut/letters-0.index

$sharedOut/indexcmp $sharedOut/output/letters-0.index testOut/letters-0.index

./indextest $sharedOut/output/letters-0.index testOut/newLetters-0.index

$sharedOut/indexcmp $sharedOut/output/letters-0.index testOut/newLetters-0.index

# letters testing with depth 10
./indexer $sharedOut/output/letters-10 testOut/letters-10.index

$sharedOut/indexcmp $sharedOut/output/letters-10.index testOut/letters-10.index

./indextest $sharedOut/output/letters-10.index testOut/newLetters-10.index

$sharedOut/indexcmp $sharedOut/output/letters-10.index testOut/newLetters-10.index

# toscrape testing with depth 0
./indexer $sharedOut/output/toscrape-0 testOut/toscrape-0.index

$sharedOut/indexcmp $sharedOut/output/toscrape-0.index testOut/toscrape-0.index

./indextest $sharedOut/output/toscrape-0.index testOut/newToscrape-0.index

$sharedOut/indexcmp $sharedOut/output/toscrape-0.index testOut/newToscrape-0.index

# toscrape testing with depth 1
./indexer $sharedOut/output/toscrape-1 testOut/toscrape-1.index

$sharedOut/indexcmp $sharedOut/output/toscrape-1.index testOut/toscrape-1.index

./indextest $sharedOut/output/toscrape-1.index testOut/newToscrape-1.index

$sharedOut/indexcmp $sharedOut/output/toscrape-1.index testOut/newToscrape-1.index

# wikipedia testing with depth 0
./indexer $sharedOut/output/wikipedia-0 testOut/wikipedia-0.index

$sharedOut/indexcmp $sharedOut/output/wikipedia-0.index testOut/wikipedia-0.index

./indextest $sharedOut/output/wikipedia-0.index testOut/newWikipedia-0.index

$sharedOut/indexcmp $sharedOut/output/wikipedia-0.index testOut/newWikipedia-0.index

# wikipedia testing with depth 1
./indexer $sharedOut/output/wikipedia-1 testOut/wikipedia-1.index

$sharedOut/indexcmp $sharedOut/output/wikipedia-1.index testOut/wikipedia-1.index

./indextest $sharedOut/output/wikipedia-1.index testOut/newWikipedia-1.index

$sharedOut/indexcmp $sharedOut/output/wikipedia-1.index testOut/newWikipedia-1.index

# Testing with valgrind
# letters testing with depth 0
$check ./indexer $sharedOut/output/letters-0 testOut/letters-0.index

$check ./indextest $sharedOut/output/letters-0.index testOut/newLetters-0.index

# letters testing with depth 10
$check ./indexer $sharedOut/output/letters-10 testOut/letters-10.index

$check ./indextest $sharedOut/output/letters-10.index testOut/newLetters-10.index

# toscrape testing with depth 0
$check ./indexer $sharedOut/output/toscrape-0 testOut/toscrape-0.index

$check ./indextest $sharedOut/output/toscrape-0.index testOut/newToscrape-0.index

# toscrape testing with depth 1
$check ./indexer $sharedOut/output/toscrape-1 testOut/toscrape-1.index

$check ./indextest $sharedOut/output/toscrape-1.index testOut/newToscrape-1.index

# wikipedia testing with depth 0
$check ./indexer $sharedOut/output/wikipedia-0 testOut/wikipedia-0.index

$check ./indextest $sharedOut/output/wikipedia-0.index testOut/newWikipedia-0.index