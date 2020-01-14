# Core

Here we describe our other core data structures. Most of them for the time being are re-mapped C++ data structures, though we work to obfuscated this with abstract interfaces.

## Symbol Table

One of our key runtime structures the symbol table has a simple job. Map strings to some sort of data. This job is further complicated by the fact that these names are sometimes meant to be name spaced using certain symbols. For this reason our symbol table is it's own set of interrelated classes.

* The symbol mapper maps strings to an incrementing integer (the "symbol").
* The symbol dictionary takes those integers ("symbol"s) and returns the data stored in the dictionary.
* The symbol table can take a sequence of symbols and answer with either no result, the data stored, or not enough symbols.

