New definition syntax. Based off of literate programming being turned into ASTs (more or less...).

Contained within `syn::def` see `defines_v2.md`.

---

Rebuild instances so they can be used to point at the Graph. Notably an instance has 3 parts:

* The type description.
* The memory management strategy.
* The location in memory of the object.

A generic instance like `instance<>` can have anything for any of those categories. Hence `instance<>` requires a pointer to a universal header object.

For the system graph we will use `instance<void, SystemGraphManager>` which will assume we have a pointer into the thread local system graph (likely the global one). Importantly if we have an instance from some other graph we will have to rely on `instance<void, void>` and see it promoted.

We should also document the difference between RawType and Object treatment by the system. As well as the ideal way Features would be handled if we could improve somethings.

---

Multimethods are objects, but so are their components:

* Per argument specializers, each argument is an instance with (potentially) a specializer property (e.g. containing an instance) attached.
  * One specializer kind could be types.
  * Another could be values / eql specialization (or even arbitrary functions).
* Potentially connected through a generic type argument to the method.
* The method is an object.

A definition could look like:

```c++
syn::define<> MyExtensionMethods(
    ACppMultiMethod
    
    << (syn::def::method)
        [](BareClass* a, syn::instance<WrappedClass> b, syn::value<int> c) -> 
        {
        
        }
        + syn::def::arg<2>::value_specializer(...)
);
```

---

Properties/Slots are objects:

* The node for slot contains a type description, access rules, etc.
* The edge tying it to objects describes the path by which it can be accessed.
  * Either direct or indirect.

---

Memory Management

See this: http://cone.jondgoodwin.com/memory.html

---

Fast runtime checks:

- [ ] Subtype: 
  - https://www.researchgate.net/publication/221552851_Fast_subtype_checking_in_the_HotSpot_JVM