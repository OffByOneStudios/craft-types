Here we the features provided within the type system.

## Names

Graph nodes have a `name` property which is a non-unique name. C++ graph nodes have a `cpp.name` property which is canonical C++ names for the nodes. Type nodes have an indexed `type.name` property that describes the unique, index-able, and canonical name of the type (which we still need to determine).

## Copying

Currently implemented by the legacy provider `PClonable` with implementation `DefaultCopyConstructor`. This provides an instance copy.

Thoughts on feature interfaces:

* kinds of memory copy: "instance copy", "place yourself there", "give me a void* and size".
* kinds of semantic copy: "clone just me", "clone my whole tree"

