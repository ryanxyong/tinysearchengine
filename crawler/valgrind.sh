#!/bin/bash
#
# valgrind.sh - valgrind tests various cases for crawler
#
# Ryan Yong, April 2021
#

check="valgrind --leak-check=full --show-leak-kinds=all"

# Valgrind testing cases with valid inputs
# letters testing with depth 0
$check ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testOut/test-0 0

# letters testing with depth 10
$check ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testOut/test-1 10

# toscrape testing with depth 0
$check ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testOut/test-2 0

# toscrape testing with depth 1
$check ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testOut/test-3 1

# wikipedia testing with depth 0
$check ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testOut/test-4 0
