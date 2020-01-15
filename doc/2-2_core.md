# Core

The first collection of types we describe are meant to be simple as possible, while being expressive enough to describe themselves. This is to allow us to create the self hosting type system in as few constructs as possible.

We place these types in `syn::core` which is the core graph of the syndicate system. We also place these types in the runtime namespace `/Syndicate/` (most of the following will be in the module `c++:syn::core` or perhaps later `system:syndicate:core`).

Because we have a meta type system, we need a way to describe the element type of our initial additions to the type graph:

* `syn::core::NEmpty` (`/Syndicate/Empty`) An empty type. Because these initial types have no information in them, merely their existence is the information, we need a way to represent them. Relationships can be made to them later to make them meta-interperable. This is represented in C++ by a structure with a private `uintptr_t` structure labeled `_reserved`.
* `syn::core::EInstanceOf` (`/Syndicate/InstanceOf`) An edge which points from a node to the type it should be understood to be an instance of. This is useful in future situations where we want to later change the perceived instance type from the actual instance type of a node in the type graph (e.g. because we made it `Empty`). (the type of this edge-type is `Empty`)

It is important to remember that we describe types here, the type of the abstract type is empty because an instance of the abstract type has no required information. The type of the signature-has-parameter edge is a struct because an instance of the signature-has-parameter will have structured data. While we could abuse bits and reference types for single member structures we choose not to except for properties for simplicity of implementation. Implementation wise these types all reduce down to the same memory and have only semantic differences.

## Abstract

Our first real type descriptions are abstract types. These are simply nodes which we can attach symbols and logic to later. These are the primary representation of generally generic types (e.g. interfaces). Relationships to complex rules can be added to them later which can enforce what can and can not be considered a part of the abstract type. Because we boot this part of the system manually we will have to be careful to not violate any of these future rules for the types we add here.

In general we assume a pretty simple rule: if a type can find a path between itself and the target abstract node then it fits. This rule moves the implementation burden to extensions to ensure they add their functionality correctly, but it simplifies bootstraping, self-hosting, and the core of the meta graph.

* `syn::core::NAbstract` (`/Syndicate/Abstract`) an abstract type (`Empty`, is-a `Type`)
* `syn::core::EIsA` (`/Syndicate/IsA`) An edge which points from a type to the abstract types it can be used as (note that this says *nothing* about structural inheritance, casting memory, or the like, simply that the type - interpreted correctly - can be use in the place of another), can only point to an abstract (e.g. two concrete types can not support an IsA relationship by default; and *never should even by extensions*, extension of IsA is reserved only to create new forms of abstract types, concrete to concrete IsA relationships should instead be represented by a more complex relationship (perhaps structural or virtual inheritance?) and the `is-a` dispatcher overloaded to support it). (`Empty`)

We also generate some system abstracts and add them to the graph:

* `/Syndicate/Meta` (`Abstract`) The meta types, these represent types intended primarily for usage within the type graph itself.
    * `/Syndicate/MetaNode` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Nodes.
    * `/Syndicate/MetaEdge` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Edges.
    * `/Syndicate/MetaLabel` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Labels.
    * `/Syndicate/MetaProp` (`Abstract`, is-a `Meta`) The meta type marking a type as used for Props.
* `/Syndicate/Type` (`Abstract`) The actual types, these describe types potentially outside the typegraph. This could be considered the top type.
* `/Syndicate/Subroutine` (`Abstract`) The actual potentially executable subroutines.

We then fix up the relationships by adding the following:
* An `IsA` edge from `InstanceOf` to `MetaEdge`.
* An `IsA` edge from `IsA` to `MetaEdge`.

Let's briefly describe our graph structure at this point (as the self-supporting structure can be confusing, and laying it out illuminating):
* `Abstract` is a node and the node's typenode is `Empty`
* `Type` is a node and the node's typenode is `Abstract`.
  * ditto `Subroutine`, `MetaEdge`, `MetaLabel`, and `MetaProp` (in place of `Type`).
* There is a node `IsA` and the node's typenode is `Empty`
* There is an edge from `MetaNode` to `Meta` and the edge's typenode is `IsA`.
  * ditto `MetaEdge`, `MetaLabel`, and `MetaProp` (in place of `MetaNode`).
* There is an edge from `Empty` to `MetaNode` and the edge's typenode is `IsA`.
* There is an edge from `IsA` to `MetaEdge` and the edge's typenode is `IsA`.

### Example Usage

An example of usage at this point is for example:

* Type `Foo` an instantiate-able class type. Represented by graph node `/User/Foo`.
  * Which `IsA` `/Syndicate/Object`
    * Which `IsA` `/Syndicate/Type`
  * The graph node's type is something like `/Syndicate/Class`
    * Which `IsA` `/Syndicate/Type`
    * Which `IsA` `/Syndicate/MetaNode`

## Type Description

Now we construct the mostly universal signature and compositional relationships. These systems will usually be used regardless of the actual implementation of these systems.

First we create our workhorse edge:

* `syn::core::EUsesType` (`/Syndicate/UsesType`) an edge for composite types which points to a type (or type description) used by a composite type description. How specifically the other type is used is determined by the properties on this edge. (`Empty`, is-a `MetaEdge`).

### Signature

First we need a way to describe signature types in their generic state:

* `syn::core::NSignature` (`/Syndicate/Signature`) a node for function signatures which describes the types involved, other features are not. For example the memory safety, calling convention, or passing of individual parameters (or their returns) are not part of this. This is meant to be used for pure type driven dispatch and inference. And while the existence and type of variable or keyword arguments would count as part of the signature, certain aspects (such as their storage mechanism) are not. Could also include optional parameters in the future as well. Other graph types can be layered on top of this with additional nodes and edges. (`Struct`, is-a `MetaNode`).
  * `parameter_count` 8 bits, number of parameters (this is the authoritative value, there must be matching indexes attached to be valid).
  * `return_count` 8 bits, number of returns (this is the authoritative value, there must be matching indexes attached to be valid). If 0, void, no return. If 1 the return type is singular in all cases. If more than 1, it will likely be automatically backed to a tuple at a higher level.
  * `varargs_index` (reserved, future) 8 bits, the index of the var-args parameter (255 is NA).
  * `kwargs_index` (reserved, future) 8 bits, the index of the kw-args parameter (255 is NA).
* `syn::core::PSignatureParameter` (`/Syndicate/SignatureParameter`) A property placed on `UsesType` edges to describe the parameter index in the signature the type being pointed at is used at, can be repeated for multiple parameters. (`UInt8`, is-a `MetaProp`).
* `syn::core::PSignatureReturn` (`/Syndicate/SignatureReturn`) A property placed on `UsesType` edges to describe the return index in the signature the type being pointed at is used at, can be repeated for multiple parameters. (`UInt8`, is-a `MetaProp`).

Now we can attach them to subroutines:

* `syn::core::EHasSignature` (`/Syndicate/HasSignature`) An edge which describes what the signature of a function is, points from an `IsA` `Subroutine` descendent to a `Signature` (`Empty`, is-a `MetaEdge`).

This is of course only a part of what is actually required to call a function, which we will get to soon.

### Structure

We will also need to be able to describe structure layout:

* `syn::core::NLayout` (`/Syndicate/Layout`) a node for structural layout which describes the types involved, other features are not. For example const-ness or volitile-ness are not considered, neither is alignment. This is meant to be used for pure structural typing (where by sequence structure or keyword anme-structure). And while the order, unioness, and bit width of fields might count as part of the layout, certain aspects (such as tagged union storage, alignment of bit fields, or immutability) are not. Other graph types can be layered on top of this with additional nodes and edges. Appropriate for use with dynamic objects with some structural information (ala dictionary object in python) if linked to with the appropriate properties. (`Struct`, is-a `MetaNode`).
  * `field_count` 16 bits, number of fields (sequence layout)
  * `entry_count` 16 bits, number of fields (keyword layout).
* `syn::core::PLayoutField` (`/Syndicate/LayoutField`) A property placed on `UsesType` edges to describe the field index in the layout the type being pointed at is used at, can be repeated for multiple parameters. (`UInt16`, is-a `MetaProp`).
* `syn::core::PLayoutEntry` (`/Syndicate/LayoutField`) A property placed on `UsesType` edges to describe the entry keyword in the layout the type being pointed at is used at, can be repeated for multiple parameters. (`Symbol`, is-a `MetaProp`).

Now we can attach them to type:

* `syn::core::EHasLayout` (`/Syndicate/HasLayout`) An edge which describes what the layout of a structure is, points from an `IsA` `Type` descendent to a `Layout` (`Empty`, is-a `MetaEdge`).

### Generics

...

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
