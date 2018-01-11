This folder contains the objects representing the possible type expressions in the type system.

Types fall into Some hierarchies (as expressed by `SType`):

* Identifiable:
  An identifiable type will have a fixed identifier in the type register. It will represent something that can be named, and whose membership (e.g. sub-typing wise) will not change in external locations.

  For example, an abstract tag may aquire new membership, but that notification will happen using the tag as an identifier. An expression that contained that tag, placed in an identifier, would not be an identifier that would be notified (due to the complex nature of expressions).
* Concrete:
  The type is complete (e.g. contains no type variables), with a known memory size. It can be instantiated.
* Abstract
  The type is a member of the type graph only.

Major Features / Not Features:

* Supports subtyping.
  * "is x a subtype of y" can be yes, no, hard, undecidable.
  * Objects can advertise direct subtype relationships.
  * Parents keep track of subtypes.
* Supports validation.
  * "is x valid" can provide a list of problems with the structuring of expressions and types.
  * "is x valid (with subtyping)" can provide a list of problems with x, but uses sub-typing to check if parts of the expression are valid (e.g. communicative types are sequenced correctly, parametric constraints are valid).
* Supports parametric types.
  * Objects can advertise direct parametrically instantiated from relationships.
  * Parametric types know their instantiated children.
* Supports type expressions.
  * Union, Exits, Tuples, Variance
* Not supports structure, but maybe.
  * Supports an idea of a "concrete" type, that is a type whose structure is sufficiently known (not consumer with open variables, not abstract, not complex expression).
* Not supports relevant functions.
  * Supports a function type expression.
  * Does not interact with the list of actual functions.
  * Possible to build a custom abstract type based off of the real functions.
* Not supports metadata.
  * Supports a convenient type identifier for identifiable types.
  * Does not interact with the metadata directly.
  * Possible to add the metadata types as abstract types (where subtype checks for applicable metadata).

### Files

* Interfaces
  * `SType`
    The core interface for all types.
* Special Type Expressions
  * `TupleType`
    A product/list of types. Can be used to represent function argument types, structure types, and parametric types.
    Types can identify themselves as "spread" types, which "spread" into the tuple.
    * `VariableTupleType`
      Infinietly repeating type.
    * `CommunicativeTupleType`
      Holds variant types 
  * `AnyType`
  * `UnionType`
    An XOR of types, that can be inhabited by only a single type.
  * `VariantType`
    A wrapper for a type that provides co/contra/bi/in-variance for the wrapped type.
  * `TypeVariableType`
    A type variable for use in parametric descriptions, and more complex queries.
  * `ExistsType`
    An existential query type, or "union all", or "where" type for use in parametric type expressions. Uses type variables.
* C++ Types
  * `CppNativeActualType`
    A real C++ type that we are wrapping without any support (e.g. no control over it, like we do with `CppObjectActualType`). Can support parametric types through pre-compiled registered types.
    * `CppObjectActualType`
    A variant of actual type that relies on the OBJECT features.
  * `CppNativeAbstractType`
    Used to represent an abstract type that can't be instantiated. Can also be parametric (will be pre-compiled at usage site).
* System Types
  * `BaseType`
    A base type for use by most types in the graph.

    Provides an implementation for the parent/child relationship.
  * `StructuredActualType`
    Effectively an inspectable (e.g. `compiler can choose`) concrete type designed to be runtime built.
  * `ParameterizedType`
    A parametric type with all type variables assigned. This can be used as a concrete type when wrapped around a parameterized one.