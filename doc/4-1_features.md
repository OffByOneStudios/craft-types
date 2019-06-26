Here we the features provided within the type system.

## Names

Graph nodes have a `name` property which is a non-unique name. C++ graph nodes have a `cpp.name` property which is canonical C++ names for the nodes. Type nodes have an indexed `type.name` property that describes the unique, index-able, and canonical name of the type (which we still need to determine).

## Copying

Currently implemented by the legacy provider `PClonable` with implementation `DefaultCopyConstructor`. This provides an instance copy.

Thoughts on feature interfaces:

* kinds of memory copy: "instance copy", "place yourself there", "give me a void* and size".
* kinds of semantic copy: "clone just me", "clone my whole tree"


# TODO

* Fix parents in define helper (some sort of auto parent wrapper implementation for singletons and factories).
  * Some sort of "expects" or something? To assert when a child didn't add features a parent needed.
* Add a `more-specific` multimethod for sorting type expressions (namely *for* multimethods). Let the dispatchers take different more specific functions. For the instance dispatcher, wrap the other dispatcher using indirection, also take a `matches` callable.
* Build some sort of universal callable feature (maybe steal PSubroutine?) with some nice introspection features for compilers, and good performance capabilities.
* Static/Dynamic property for nodes (e.g. is this node created at runtime by an interpreter or is it static).
* Add function and variable nodes.
  * Variables and functions need a way to reference types (e.g. what type is the variable, the arguments, the return type).
    * Could be a common edge... would require work on the graph to keep performance acceptable. This is the most "pure" representation. Only works for concrete descriptions though...
    * Many variables/arguments/return types are some sort of complex type. Perhaps using a property and an expression is the correct way to go.
    * Perhaps use the expression graph by default, and add the concrete edges as a feature later?
  * Need storage specifiers (e.g. instance, pointer, reference). As some sort of subgraph? depends how we do arguments and type references for these. Probably a property with a reference to a node.
* Add neo-features system.
  * Good first implementation candidates: Anything in boot (stringer and parser come to mind), storage specifiers, others.
  * Needs a way to be ergonomic for C++ (e.g. eliminate adding the `this` pointer every time; make it easy to declare/define).

