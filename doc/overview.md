This is a runtime type system. It has the following key features:

* Expressive, extensible, and powerful graph of types for runtime queries.
* C++ template based interface which can allow for compile time queries.
* Core library of common features.

## Philosphy

* Power first.
* Performance second.
* Don't pierce the compiler's veil (for stability reasons).

## Design

The following singletons (global and thread local for in process isolated threads) provide the key features of the type system:

* `TypeGraph& craft::types::graph()` the graph for runtime type queries.
* `Identifiers& craft::types::identifiers()` the list of identified types, their identifiers, and their meta data. Also stores identifiers for features.
* `CppSystem& craft::types::system()` the compile/runtime helper interface for c++.

### Expressions

Expressions are managed by the `TypeGraph`. For performance the graph attempts to de-duplicate types. The graph manages the memory of types (even though types are themselves types in the type system).

### TypeId

TypeId's are system word sized integers that represent a type. They can either by increasing integers, or simply pointers to the type objects that represent them in the type graph. The only guarantee they need to provide is with an interface and that equality means they are the same concrete type.

### Instances

Instances are allocated memory of a type, interpreting the memory obviously depends on the type. In general the memory does not provide a mechanism for recovering the type, or other helpers.

There is a standard instance metadata struct. It is composed of the TypeId, the pointer to the actual instance, a reference count (for reference counted types). It *does* provide recovering the type, and wrapping arbitrary instances.

### Features

Feature is a broad term describing properties of types, relationships between types, and identities. A feature (the one with a TypeId) is a concrete type which is represented by the memory (primarily function pointers) required to work with/use the feature.

Fundamentally, features provide a concrete object given some information. However a feature with the same type may exist multiple times as distinct identities.

Features have different names depending on how many objects they interact with:

* Provider: One or more types, rarely any live objects (e.g. for constructing new instances, or perhaps a method is "aspect" like).
* Aspects: One type / live object of that type.
* Multimethod: An interface of a single method of arbitrary amounts instances/types. Usually heavily named.

### Cpp Interface

The primary interface is through the `instance<>` class and a set of helper macros.

#### Instance

An `instance<>` is a form of smart pointer.

An instance with no arguments represents the "any" type, to be explicit one can use the unconstructable `Any` class.

The first argument to an instance is the type expression. If this type is concrete (e.g. because it's a class, feature, or templated class entered into the type system) the instance will be able to generate efficient code, and ensure anything placed into it is of that type.

The second argument determines the shape of the instance itself. By default it uses the legacy dual pointer design.

The last argument is used to pass meta data about the instance (like lifecycle information, that may allow the compiler to elide reference counting).

An `instance<>` with type information (metadata may differ) in a function's (arguments) signature is expressing caller guarantees, casting the type after it is received is a callee check.

##### Interface

* 

#### Object

Objects are the 

#### Features

Representing features with CPP is tricky. Ideally they would be represented with virtual function tables, but those are not accessible in a stable way. There are legacy feature macros which use this style, while accesible through the type system, they are deprecated.

Features are represented by a templated class, which represents the feature. The template arguments are type expressions. 

### Lisp

#### Optimizations

A list of optimizations around this type system the lisp will be able to make that c++ will not be able to:

* The lisp doesn't have to make any guarantees about the shape of memory unless it is asked too.
* The lisp can jit code for different scenarios.