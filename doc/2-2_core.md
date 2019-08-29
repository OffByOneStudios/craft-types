

The first collection of types we describe are meant to be simple as possible, while being expressive enough to describe themselves.

## Core

We place these types in `syn::core` which is the core graph of the syndicate system. We also place these types in the runtime namespace `/Syndicate/Core/` (which is later forwarded to `/Syndicate/`).

Because our we have a meta type system, we need a way to describe the element type of our initial additions to the type graph. We create a node called empties because these have no information in them, merely their existence is the information.

* `syn::core::NEmpty` (`/Syndicate/Core/Empty`) Empty.

### Abstract

First we must describe an abstract type. These are simply nodes which we can attach symbols to later. They are our first representation of a type, and the primary representation of types that can be filled in later.

In the future they can have complex rules associated with what can and can not be considered a part of the abstract type. At the moment we will have to be careful to ensure we do not violate any rules we rely on because of these abstracts.

For now we use a simple rule, if a type can find a path between itself and the target abstract node then it fits.

* `syn::core::NAbstract` (`/Syndicate/Core/Abstract`) an abstract type. Empty.
* `syn::core::EIsA` (`/Syndicate/Core/IsA`) An edge which points from a type to the other types it can be used in place of (note that this says *nothing* about structural inheritence, casting memory, or the like, simply that the type - interpreted correctly - can be use in the place of another), can only point from a "type" (instance-inheriter of `Meta`) to an abstract. Empty.

We also generate some system abstracts and add them to the graph:

* `/Syndicate/Core/Meta`  (`Abstract`) The meta type graph, these describes types found with in the type graph.
* `/Syndicate/Core/MetaNode` (`Abstract`, is-a `Meta`)
* `/Syndicate/Core/MetaEdge` (`Abstract`, is-a `Meta`)
* `/Syndicate/Core/MetaLabel` (`Abstract`, is-a `Meta`)
* `/Syndicate/Core/MetaProp` (`Abstract`, is-a `Meta`)
* `/Syndicate/Core/Type`  (`Abstract`) The actual types, these describe types potentially outside the typegraph.
* `/Syndicate/Core/Subroutine`  (`Abstract`) The actual potentially executable subroutines.

Let's briefly describe our graph structure at this point (as the recursive structure can be confusing):
* `Abstract` is a node and the node's typenode is `Empty`
* `Meta` is a node and the node's typenode is `Abstract`.
  * ditto `MetaNode`, `MetaEdge`, `MetaLabel`, and `MetaProp`.
* There is a node `IsA` and the node's typenode is `Empty`
* There is an edge from `MetaNode` to `Meta` and the edge's typenode is `IsA`.
  * ditto `MetaEdge`, `MetaLabel`, and `MetaProp`.
* There is an edge from `Abstract` to `MetaNode` and the edge's typenode is `IsA`.
* There is an edge from `IsA` to `MetaEdge` and the edge's typenode is `IsA`.

### C Memory

Now we can support C style memory. This involves a node that represents structures, and one for bits types. As well as an initial population of pointer types.

* `syn::core::NBits` (`/Syndicate/Core/Bits`) a node for a bits type (that is any type that takes up some number of bits), this is our first concrete type (`Struct`, is-a `MetaNode`).
* `syn::core::NStruct` (`/Syndicate/Core/Struct`) a node for a struct type (that is any type that takes up a chunk of memory via composition) (`Struct`, is-a `Bits`).
* `syn::core::NReference` (`/Syndicate/Core/Reference`) a node for an in memory reference type (that is any type points to memory of another) (`Empty`, is-a `Bits`).

Now we generate some system helpers:

* `/Syndicate/Core/Void` a node for zero width types (`Bits` 0, is-a `Type`).

* `syn::core::EPointsAt` (`/Syndicate/Core/PointsAt`) An edge which describes what a reference-esque type instance would point at in memory (e.g. for dereferenceability) can be repurposed by other single object dereferencable containers (`Empty`, is-a `MetaEdge`).

* `/Syndicate/Core/UniversalReference` a node for an arbitrary pointer (`Reference`, is-a `Type`, points-at `Void`).

### C Function

Now we can support C style functions.

* `syn::core::NSignature` (`/Syndicate/Core/Signature`) a node for function signatures, all arguments and returns are considered part of the signature (though their meta data may not be, for example varargs and kwargs are merely recognized for existence, not for shape) however calling convention, type safety, and other features are not (`Empty`, is-a `MetaNode`).
* `syn::core::NFunction` (`/Syndicate/Core/Function`) a node for a c-style function, this is our first subroutine node type (`Reference`, is-a `Subroutine`, points-at "assembly").

(for example in the future a we might have a `RestRpc` of memory type `RestRoute` or even `String` that then is-a subroutine)

* `syn::core::EHasSignature` (`/Syndicate/Core/HasSignature`) An edge which describes what the signature of a function is, points from a subroutine descendent to a signture (`Empty`, is-a `MetaEdge`).
* `syn::core::EWithArgument` (`/Syndicate/Core/WithArgument`) An edge which describes the argument of a function signature (`Struct`, is-a `MetaEdge`).
* `syn::core::EHasReturn` (`/Syndicate/Core/HasReturn`) An edge which describes the return value of a function signature (`Empty`, is-a `MetaEdge`).