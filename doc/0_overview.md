This is a runtime type system. It has the following key features:

* Expressive, extensible, and powerful graph of types for runtime queries.
* C++ template based interface which can allow for compile time queries.
* Core library of common runtime features (type conversion, construction, member access).

### Philosphy

Our guiding values for our design (in order)

1. Power - capability of expression - is paramount.
2. Performance - speed of execution and use of memory - is important.
3. Don't pierce the compiler's veil (for stability).

## Overview

1. **Runtime Structures**
   
   The runtime structures are a collection of helper functions (and definitions) for certain data structures required for interfacing with the runtime system (in the first place).
   
2. **Definition Data Model**

   This is a graph model of a system of definitions as well as the core definition model types. Including utilities for adding and querying information, like the definition method chaining libraries. Using this one can generate new definitions for any system (including creating their own definition model types for arbitrary extension) as well as use the runtime library to back any system. This is the core of the library.

3. **C++ Integration**

   This is a C++ native integration (through use of templates) of the runtime system. This allows for the easy (macro free) definition of runtime modules (scoped to DLLs or arbitrarily), types, and functions through static initialization.

4. **Usage**

   We then discuss usage scenarios, overviews, and examples.

### 1. Runtime Structures

In no particular order:

* Symbol tables
* Vectors and Maps
* Universal data structure
* Instance helpers
* Data structure specialization utilities (for providing semi-optimized data structures that "share" an instanced header across their internal instances)



### 2. Definition Data Model



### 3. C++ Integration



### 4. Usage

