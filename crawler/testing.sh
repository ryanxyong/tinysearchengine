#!/bin/bash
#
# testing.sh - tests various cases for crawler
#
# Ryan Yong, April 2021
#

mkdir testOut
sampleOutPath="/thayerfs/home/f004gx3/cs50-dev/shared/tse/output"

# Test cases for argument bounds
# No arguments provided
mkdir testOut/test-0
./crawler

# One argument provided
mkdir testOut/test-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/

# Two arguments provided
mkdir testOut/test-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/ testOut/test-2

# Three arguments provided but invalid depth
mkdir testOut/test-3
./crawler http://cs50tse.cs.dartmouth.edu/tse/ testOut/test-3 -1

# Three arguments provided but invalid depth
mkdir testOut/test-4
./crawler http://cs50tse.cs.dartmouth.edu/tse/ testOut/test-4 11

# Three arguments provided but invalid seedURL
mkdir testOut/test-5
./crawler ryanyong.com testOut/test-5 1

# Three arguments provided but invalid pageDirectory
mkdir testOut/test-6
./crawler http://cs50tse.cs.dartmouth.edu/tse/ bill 1

# Testing cases with valid inputs
# letters testing with depth 0
mkdir testOut/test-7
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testOut/test-7 0
diff -r testOut/test-7 $sampleOutPath/letters-0

# letters testing with depth 10
mkdir testOut/test-8
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testOut/test-8 10
diff -r testOut/test-8 $sampleOutPath/letters-10

# toscrape testing with depth 0
mkdir testOut/test-9
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testOut/test-9 0
diff -r testOut/test-9 $sampleOutPath/toscrape-0

# toscrape testing with depth 1
mkdir testOut/test-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testOut/test-10 1
diff -r testOut/test-10 $sampleOutPath/toscrape-1

# wikipedia testing with depth 0
mkdir testOut/test-11
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testOut/test-11 0
diff -r testOut/test-11 $sampleOutPath/wikipedia-0

# wikipedia testing with depth 1
mkdir testOut/test-12
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testOut/test-12 1
diff -r testOut/test-12 $sampleOutPath/wikipedia-1
