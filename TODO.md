# Fast match
* -F|--fast
* stream-oriented
* requires sorted input like uniq
* sensitive to early edits that would cause sort to break up clusters, if sorting with naive
  lexical commparison
* advantage: compare only the current line to the previous
* compare to previous LINE or previous KEY?
  * e.g. every line D distance from previous line, or all lines D distance from a single,
    coincidentally-first cluster member?
  * previous KEY! See comment in Matcher.h
* How should -a work?
  * so far, print exemplar, then tab-indented copies below, then count-line if applicable
* honor -u/-d

# stats
* number of comparisons
* time
* time per comparison
* number of comparisons per match
* number of mismatches found
* stats to stderr even on ctrl-c

# uniq compatibility
* -d: Only output lines that are repeated in the input.
* -f num: Ignore the first num fields in each input line when doing comparisons.
* -s chars: Ignore the first chars characters in each input line when doing comparisons.
* -u: Only output lines that are not repeated in the input.

# FastMap
* FastMap requires an entire dataset to be in memory to find useful axes.
* funiq could specifically assume uniformly unsorted input.
* Given a prefix of the input stream (some large amount that can be temporarily stored in memory):
  * Search for pivot elements/axes within prefix
  * Find coordinates for all other elements along these axes
  * Count and track like normal `funiq` batch mode

# Online FastMap
* FastMap traditionally depends on access to an entire dataset to determine pivot elements for dimensional axes.
* Instead, try finding axes using prefix of dataset, but allow gradual updating/annealing of axes.
* When finding a new element near (to some epsilon) an axis, but further along than one pivot element for that axis:
  * replace that pivot element
  * recompute all coordinates to that axis
  * recompute all clustroids
  * DO NOT recompute cluster memberships

# additional ideas
* average match distance on each cluster
* -a print every line separated by \n\t, with distance from exemplar
* agrep/adist matching?
  * agrep uses Bitap Algorithm
  * http://www.drdobbs.com/fuzzy-text-searches-with-agrep-and-afind/199101191
  * https://www.tgries.de/agrep/
* other distance metrics besides levdist
  * MinHash
  * Dice Coefficient
    * http://cdunix.blogspot.com/2008/10/fuzzy-uniq-or-parsing-connectdirect.html
    * http://www.catalysoft.com/articles/StrikeAMatch.html
    * https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Dice%27s_coefficient
  * http://ntz-develop.blogspot.com/2011/03/fuzzy-string-search.html
* tests
* ignore-numeric
* ignore-charset
* record delimiter, a la `agrep -d`