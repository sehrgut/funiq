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

# additional ideas
* agrep/adist matching?
  * agrep uses Bitap Algorithm
  * http://www.drdobbs.com/fuzzy-text-searches-with-agrep-and-afind/199101191
  * https://www.tgries.de/agrep/
* other distance metrics besides levdist
  * MinHash
  * http://ntz-develop.blogspot.com/2011/03/fuzzy-string-search.html
* tests
* ignore-numeric
* ignore-charset
* record delimiter, a la `agrep -d`