The core of the definition data model is the runtime type graph. The purpose of this graph is to represent all definitions and their relationships in the runtime system. This is the primary data structure through which different parts of the system communicate.

We make some decisions about the structure of the graph:

* It is a runtime structure, so caching (and duplicating) data is acceptable.
  * Cache validation can be checked against atomically incremented revision tags.
* It is a typed graph, where the types are themselves placed into the graph.
* We use a naming standard for our graph types:
  * `NFooBar` is a node, and should be a noun phrase.
  * `EWantsFoo` is an edge, and should be a relationship.
  * `LFooBar` is a label, and should be a noun phrase.
  * `PFooBar` is a property, and should be a statement.

## Definitions

There are two crucial kinds of definitions: **types** and **subroutines**. These represent the core of any system, **types** are descriptions of objects, and **subroutines** are some sort of computation (in general something executable). Other nodes can be placed in the graph, but in general they will at least have either a type label or a subroutine label.

The first problem is bootstrapping the system. While the goal is to provide a generic data structure capable of representing any programming system, we must first be able to describe that data structure in an easy to access way. To do this we must describe an in memory representation. And then that in memory representation must describe itself. We do this through our core description elements, which lean on C memory semantics.

## Meta System

This system is a meta system. And so the system is described within itself.

If I create a new `NCppInterface` node how do I know if that is an `NAbstract`? Simple, I follow the `EIsA` relationship. Just like I would follow an *instance* of `NStruct`'s `EIsA` relationships to see if it matches an *instance* of `NAbstract`.