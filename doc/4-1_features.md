Here we show off some of the "killer" features provided within the type system.

Get all types constructuble using a `FooType` (graph syntax is still a work in progress by another library, and this is without any convience helpers we plan on adding):

```c++
using namespace syn;
using namespace syn::core;
```

```c++
  auto convertable_types = syn::system().query()
    .v(type<FooType>())
    .in([](auto n, auto e) { return is_type<ESigntureArgumentTypes>(e->type)
      // We are the 0th and only argument; e[0] is the signature itself
      && e->nodes.size() == 2 && e[1] == n; })
    // this as-back block would be better organized as a `check()` once implemented
    // infact this could probably even be a transformer!
    .as("signature")
        .in([](auto n, auto e) { return is_type<EHasSigntureType>(e->type); })
        .hasProp([](auto p) { return is_type<PConstructor>(e->type); })
    .back("signature").unique()
    .out([](auto n, auto e) { return is_type<ESigntureReturnTypes>(e->type) })
    .run();
```


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

