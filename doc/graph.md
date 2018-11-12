The type system is a "universal middle" (e.g. LLVM IR) style architecture. It generally is composed of three parts:

* Reflection system. This part is the reflection of a source type system. It's goal is to perserve all the information it can, as effeciently as it can.
* Graph system. This part is the total graph of all relevant information. It's goal is to allow queries across types and functions.
* Artifact system. This part is the target specific output of the graph system. It's goal is to provide effecient as possible runtime execution informed by the type system.

This library contains objects composing the above system, along with some implementations:

* C++ reflector. An in code, programmer controlled, templated reflection system. This allows programmers to describe - by hand - the type data of arbitrary C++ types. Some of this information may be availble at template time as well. At the moment this writes directly into the graph, rather than a faster intermediate layer.
* C++ runtime. A library of features which can translate between (or be embedded into) the type graph and C++.
* C++ templated artifacts. A library which combines templated reflector information with the C++ runtime to provide suffciently effecient artifacts for interacting with arbitrary external types while providing fast paths for local natively compiled code.

# OLD

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

* `cpp.class`: An external (likely POD) C/C++ type.
* `cpp.object`: A type using our object helper.
* `cpp.feature|legacy`: A legacy inheritance / virtual function feature.
* `cpp.method`: C++ method for a class.
* `cpp.function`: C/C++ function. The source of a `cpp.info-of` edge if it is bound to a type someway.
* `cpp.variable`: C/C++ variable. The source of a `cpp.info-of` edge if it is bound to a type someway.
* `cpp.multi-method`: C++ static multi-method.
* `cpp.user-info`: C++ static user info (for arbitrary usage). The source of a `cpp.info-of` edge if it is bound to a type someway.
* `abstract`: An abstract type (a symbol).
* `feature`: A description of a set of features a type can have to fulfill this feature.
* `multi-method`: Our implementation of multimethod.

### Properties

* `type.id` An integer index for concrete types for performance.
* `type.name` A canonical public name for concrete types.
* `name` Name of this node (multiple nodes may share a name).
* `meta.index` Property for property meta nodes to store their index in.
* `cpp.size`: Size of this type in bytes.
* `cpp.name`: C++ name for a type

### Edges

- `is-a` An edge representing the `is-a` relationship:
  This edge has no value. This is the edge used by the `is-subtype` checker. Types which satisfy features have these, abstract tags use these. This is a directional relationship, the first type *is* the second type.
- `cpp.implements|legacy` An edge representing a legacy feature implementation.
  The value of this edge is the implementation object (factory or singleton). This is the edge used by the `getFeature` and `hasFeature` runtime checks.
- `cpp.info-of` An edge representing info of a type... might be moved out of cpp namespace.
  The value of this edge is a description of how the info is bound (static, instance) and the name of it. For example the node on the other end could be a static or member function (static or instance binding respectively), or a static or member variable (static or instance binding respectively), or some other static or instance bound info,