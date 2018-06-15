# Basics of Craft Types

This is a C++ reflection and type system. Initially built for that primary purpose, and later developed as a type system and runtime for programming languages.

The primary concept of the type system is the type graph. An in memory graph database which provides extensible reflection capabilities at runtime. The graph is populated with C++ code in DLLs at load time (through the use of macros, static initalization, and dll load hooks) and through programming language runtimes.

Nodes in the graph represent information like abstract types, concrete types, interfaces, functions, named type symbols, and other information.