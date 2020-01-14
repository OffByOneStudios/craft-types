# Core

The first collection of types we describe are meant to be simple as possible, while being expressive enough to describe themselves. This is to allow us to create the self hosting type system in as few constructs as possible.

We place these types in `syn::core` which is the core graph of the syndicate system. We also place these types in the runtime namespace `/Syndicate/Core/` (which is later forwarded to `/Syndicate/`).

Because we have a meta type system, we need a way to describe the element type of our initial additions to the type graph:

* `syn::core::NEmpty` (`/Syndicate/Empty`) An empty type. Because these initial types have no information in them, merely their existence is the information, we need a way to represent them. Relationships can be made to them later to make them meta-interperable. This is represented in C++ by a structure with a private `uintptr_t` structure labeled `_reserved`.
* `syn::core::EInstanceOf` (`/Syndicate/InstanceOf`) An edge which points from a node to the type it should be understood to be an instance of. This is useful in future situations where we want to later change the perceived instance type from the actual instance type of a node in the type graph (e.g. because we made it `Empty`). (`Empty`)

## Abstract

Our first real type descriptions are abstract types. These are simply nodes which we can attach symbols and logic to later. These are the primary representation of generally generic types (e.g. interfaces). Relationships to complex rules can be added to them later which can enforce what can and can not be considered a part of the abstract type. Because we boot this part of the system manually we will have to be careful to not violate any of these future rules for the types we add here.

In general we assume a pretty simple rule: if a type can find a path between itself and the target abstract node then it fits. This rule moves the implementation burden to extensions to ensure they add their functionality correctly, but it simplifies bootstraping, self-hosting, and the core of the meta graph.

* `syn::core::NAbstract` (`/Syndicate/Core/Abstract`) an abstract type (`Empty`, is-a `Type`)
* `syn::core::EIsA` (`/Syndicate/Core/IsA`) An edge which points from a type to the abstract types it can be used as (note that this says *nothing* about structural inheritance, casting memory, or the like, simply that the type - interpreted correctly - can be use in the place of another), can only point to an abstract (e.g. two concrete types can not support an IsA relationship by default; and *never should even by extensions*, extension of IsA is reserved only to create new forms of abstract types, concrete to concrete IsA relationships should instead be represented by a more complex relationship (perhaps structural or virtual inheritance?) and the `is-a` dispatcher overloaded to support it). (`Empty`)

We also generate some system abstracts and add them to the graph:

* `/Syndicate/Core/Meta` (`Abstract`) The meta types, these represent types intended primarily for usage within the type graph itself.
    * `/Syndicate/Core/MetaNode` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Nodes.
    * `/Syndicate/Core/MetaEdge` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Edges.
    * `/Syndicate/Core/MetaLabel` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Labels.
    * `/Syndicate/Core/MetaProp` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Props.
* `/Syndicate/Core/Type` (`Abstract`) The actual types, these describe types potentially outside the typegraph. This could be considered the top type.
* `/Syndicate/Core/Subroutine` (`Abstract`) The actual potentially executable subroutines.

Let's briefly describe our graph structure at this point (as the self-supporting structure can be confusing, and laying it out illuminating):
* `Abstract` is a node and the node's typenode is `Empty`
* `Type` is a node and the node's typenode is `Abstract`.
  * ditto `Subroutine`, `MetaEdge`, `MetaLabel`, and `MetaProp` (in place of `Type`).
* There is a node `IsA` and the node's typenode is `Empty`
* There is an edge from `MetaNode` to `Meta` and the edge's typenode is `IsA`.
  * ditto `MetaEdge`, `MetaLabel`, and `MetaProp` (in place of `MetaNode`).
* There is an edge from `Empty` to `MetaNode` and the edge's typenode is `IsA`.
* There is an edge from `IsA` to `MetaEdge` and the edge's typenode is `IsA`.

## C Compatibility

### C Memory

Now we can describe C style memory. This involves a node that represents structures, one for bits types, and one for pointers (references). As well as an initial population of pointer types.

* `syn::core::NBits` (`/Syndicate/Core/Bits`) a node for a bits type (that is any type that takes up some number of bits), this is our first concrete type (`Struct`, is-a `Type`).
* `syn::core::NStruct` (`/Syndicate/Core/Struct`) a node for a struct type (that is any type that takes up a chunk of memory via composition) (`Struct`, is-a `Bits`).
* `syn::core::NReference` (`/Syndicate/Core/Reference`) a node for an in memory reference type (that is any type points to memory of another) (`Empty`, is-a `Bits`).

Now we generate some system helpers:

* `/Syndicate/Core/Void` a node for zero width types (`Bits` 0, is-a `Type`). This is not a none-type. A reference to any type with memory can be casted to a reference of this because all memory based types have at least 0 bits of memory. May have special behaviour when used as an argument or return type (is removed from the argument list) however one should consult their ABI (C++ ABIs may not erase 0 width types). In general 0 width types may be decayed to this node during optimization, failure to do so may result in perforamnce penalities.
* `syn::core::EPointsAt` (`/Syndicate/Core/PointsAt`) An edge (`Empty`, is-a `MetaEdge`) which describes what a reference-esque type instance would point at in memory (e.g. for dereferenceability) may be repurposed through extension by other single object dereferencable containers as long as they aren't of typenode `Reference`.
* `/Syndicate/Core/UniversalReference` a node for an arbitrary pointer (`Reference`, is-a `Type`, points-at `Void`) this works because the pointed at type has 0-sized memory.

### C Function

Now we can describe C style functions.

* `syn::core::NFunction` (`/Syndicate/Core/Function`) a node for a c-style function (including C++), this is our first subroutine node type (`Reference`, is-a `Subroutine`, points-at "native assembly").

## Composite Typing

Now we construct the mostly universal signature and compositional relationships (for example in the future a we might have a `RestRpc` of memory type `RestRoute` or even `String` that then is-a subroutine but uses the following relationships).

### Signature

* `syn::core::NSignature` (`/Syndicate/Core/Signature`) a node for function signatures, all arguments and returns are considered part of the signature (though their meta data may not be, for example varargs and kwargs are merely recognized for existence, not for shape) however calling convention, type safety, and other features are not (`Empty`, is-a `MetaNode`).

### Structure

* `syn::core::NLayout` (`/Syndicate/Core/Signature`) a node for structural layout, all fields are considered part of the layout (though their metadata may not be, alignment is included in layout, const-ness as purely an access rule is not).


* `syn::core::EHasSignature` (`/Syndicate/Core/HasSignature`) An edge which describes what the signature of a function is, points from a subroutine descendent to a signature (`Empty`, is-a `MetaEdge`).
* `syn::core::EWithArgument` (`/Syndicate/Core/WithArgument`) An edge which describes the argument of a function signature (`Struct`, is-a `MetaEdge`).
* `syn::core::EHasReturn` (`/Syndicate/Core/HasReturn`) An edge which describes the return value of a function signature (`Empty`, is-a `MetaEdge`).