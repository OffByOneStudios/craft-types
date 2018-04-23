At it's core the type system is an in memory graph of types. The graph has three primary concepts: a Node, an Edge, and a Property. All of these use a node as their type. If a node has a type node that is null then it is a root node. Root nodes are "stringly" typed.

Nodes represent a type or object the system knows about. Types, Functions, Features, etc. Nodes have an arbitrary number of properties and edges.

Properties represent information about a Node. Edges represent a relationship between multiple nodes. Properties and edges both have interfaces which allow the graph to reason about them as groups of things.

## Example Graphs

`cpp-type` >> `type-info: Foo`

* `property: full-cpp-name` [`some.namespace.Foo`] `
* `property: full-cpp-name` [``] 
* `edge: <--function-of--` [`toString`]

## Meta Nodes

### Nodes

* `cpp.type.class`: An external (likely POD) C/C++ type.
* `cpp.type.object`: A type using our object helper.
* `cpp.type.feature|legacy`: A legacy inheritance / virtual function feature.
* `cpp.func.method`: C++ method for a class.
* `cpp.func.function`: C/C++ function.
* `type.abstract`: An abstract type (a symbol).
* `type.feature`: A description of a set of features a type can have to fulfill this feature.
* `system.multi-method`: Our implementation of multimethod.

### Properties

* `type-id` An identifier for concrete types.
* `name` Name of this node (multiple nodes may share a name).
* `cpp.size`: Size of this type in bytes.
* `cpp.name`: C++ name for a type

### Edges

