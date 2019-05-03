The new c++ define system is designed to be macro free and works as follows.

### Kinds

There are different kinds of defines exported. These map not to graph nodes, but to concepts:

* `Extend`: Extend existing information with anything.
* `Function`: A bare function, prefer multi-method.
* `MultiMethod`: A multimethod, can contain multiple functions.
* `Class`: An object description (from plain old data to complex).
* `Concept`: A conceptual description abstract type.
* `Module`: A module, a container of defines.

### Define

First we have to declare an object:

```c++
// standalone
syn::define<CppDefineHelperType> GenericInfoObject;
```

A basic define that just provides more information to the type system would be:

```c++
syn::define<> MyDefinitions(
    [](syn::Definer<> _) {
    
    }
);
```

We can see some constructs here.

We then abuse operator overloading to give us the ability to add features to things.