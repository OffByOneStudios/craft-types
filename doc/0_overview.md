This is a runtime system. It is designed to provide the necessary features for a dynamic programming language (primarily our language Cult). To provide some of our intended features for this programming language we needed a unique runtime system to provide the necessary data-structures and support. The core of syndicate is a powerful extensible self-hosted graph of types and functions. On top of which the runtime library builds some important core features, including:

* Multimethods with arbitrary dispatchers, but also common type query dispatchers.
* Powerful abstract types, like multi-type interfaces and algebraic data types. But perhaps also features like type states and reference safety.
* Dynamic module system (for native code!) with introspection and hot-reload features built in.
* Stack frame features, like dynamic scoping and support for condition systems.

Additionally to service our core goal of a runtime it is designed to provide these features to other native languages, and especially it's host language, C++. Meaning all of the above features can be fully integrated and used from C++, in most cases in relatively ergonomic ways. As an additional layer on-top of our C++ support we support template-time optimizations allowing for efficient code generation even without runtime code generation.

This library is in some ways incomplete without the language it is meant to host included with it. Features like interactive execution, runtime code generation, dynamic objects, parsers, and so on are found within the language library (or one of it's support libraries) proper. This library does include features like a symbol table and basic data structures

Finally, the extensible graph system means - in theory - any language can write it's own runtime support into the graph and be used through the runtime system. This means the runtime can be used as universal cross-runtime support system. This is our eventual goal.

### Philosophy

Our guiding values (in order) for our design when a decision needs to be made:

1. Power - capability of expression - is paramount.
2. Performance - speed of execution and use of memory - is important.
3. Encapsulation - separation of concerns - is necessary.
  * Don't pierce the compiler's veil (for stability).

## Overview

The primary header `/src/syn/syn.h` includes the different chapters of this manual more or less in rough order, but more importantly includes a manual chapter reference for most includes. Allowing the manual and the headers to be read together in a pseudo literate programming style.

1. **Runtime Structures**
   
   The runtime structures are a collection of helper functions (and definitions) for certain data structures required for interfacing with the runtime system (in the first place). Some of these are declared before the type system and others after.
   
2. **Definition Graph**

   This is a graph model of a system of definitions as well as the core definition model types. Including utilities for adding and querying information, like the definition method chaining libraries. Using this one can generate new definitions for any system (including creating their own definition model types for arbitrary extension) as well as use the runtime library to back any system. This is the core of the library.

3. **C++ Integration**

   This is a C++ native integration (through use of templates) of the runtime system. This allows for the easy (macro free) definition of runtime modules (scoped to DLLs or arbitrarily), types, and functions through static initialization.

4. **Usage**

   We then discuss usage scenarios, overviews, and examples.

### 1. Runtime Structures

In no particular order:

* Symbol tables
* Instance helpers
* Abstract data structure types
* Vectors and Maps
* Universal data structure
* Data structure specialization utilities (for providing semi-optimized data structures that "share" an instanced header across their internal instances)
* Dispatchers
* Concurrency and Execution helpers
* Stack frame and dynamic scoping features
* Condition system objects

### 2. Definition Graph

These are more or less listed in include order.

* 2.1 Describes the overall design of the graph.
* 2.2 Describes the core self hosting definitions, including the C based pieces necessary for the library to describe itself.
* 2.3 Describes the primary extensions the Syndicate library includes.
* 2.4 Describes the C++ specific extensions needed for the C++ integration included with Syndicate to function properly.
* 2.5 Describes the additional universal features the Syndicate library adds.

### 3. C++ Integration



### 4. Usage

