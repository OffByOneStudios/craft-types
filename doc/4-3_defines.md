Here we discuss the define syntax available for C++.

## C++ Types

Here we describe the define syntax for pure C++ types (e.g. `Define`).

### name

```c++
  _.name("Foo")
```

This names the given type as the symbol "Foo". This automatically puts it in the current module as that symbol. This will also cause merge rules to run (e.g. for defining multi-methods multiple times).

### space

```c++
  _.space("/Syndicate/Math", "Foo");
```
```c++
  _.name("Foo").space("/Syndicate/Math", /*optional*/ "Foo");
```

This places the symbol into the named namespace.

Some conventions may prefer to do this in the module definition instead.

### detectLifecycle

```c++
_.detectLifecycle();
```

This automatically detects the lifecycle of the object. It is strongly recommended for all users to prefer this method of lifecycle definition.

* Calls `_.destructor` with a wrapper around the destructor if found (e.g. not deleted). Handles virtual destructors.
* Calls `_.copier` with a wrapper around the copy constructor if found.
* Calls `_.mover` with a wrapper around the move constructer if found.
* Calls `_.assigner` with a wrapper around the `=` operator (both moveable and non-moveable, with appropriate modifiers) if found.
* Calls `_.constructor` with a wrapper around the *default constructor* if found.
* Calls `_.abstract` if the class is abstract.
* Calls `_.final` if the class is final.
* If none of the above are found (besides final), sets the type as a plain old data type, with initial memory zero (e.g. as if `_.plainOldData()` was called).
* This also sets the memory size of the object with `_.size`. This behavior is optional by passing in `syn::define::DetectLifecycleFlags::NoSize`. The runtime uses size as a minimum size, and for allocation of memory. Pointers to types do not use it, and hence it is safe for a derived type to masquerade as a base type as long as the base type does not expose copy, move, or assignment operators that will break if used on the derived type.

**Note**: Users must still call `_.constructor` if they have custom constructors they want to expose (including named static ones).

### member

```c++
  _.member("Foo", &MyClass::Foo);
```

Creates a new slot node on the type of the given symbol. This is a reservation of memory within the type.

Uses the member pointer to calculate an offset for the member variable if it can (most cases). If not it will wrap it in a property syntax.

### method

```c++
  _.method("Foo", &MyClass::Foo)
```

```c++
  // For overloads
  _.method<TheReturn, TheFirstArgument, TheSecondArgument>("Foo", &MyClass::Foo)
```

* If the function is a plain class method (and fits in a function pointer of the correct type) will cast the class function pointer to a plain old function, place it in the type system, and attach it as a method on the given type.
  * If the function conflicts on symbol, a compile-time dispatch node will be created that joins the two functions together.
* If the function is a virtual function (and likely does not fit in a function pointer of the correct type) or is some other callable (and hence also won't fit) will wrap the given callable in a trampoline lambda, and place that in the type system. Marks the function as an opaque dispatcher and adds it as a member.
  * A repeat virtual function of the same signature (besides dispatch type) will cause an error.
  * If the signatures differ a compile time dispatch node is added that ignores the first argument.

### constructor

A variant of `_.method` which:

* Adds an edge property to mark it as a `Constructor` if the given function returns a pointer (or smart pointer) to the object.
* Adds an edge property to mark it as a `MemoryInitalizer` if the given function takes a `void*`, this would require the size of the object to be set. This could be done through a lambda calling placement new.

### inherits

Creats an is-a edge and add an edge propety to mark it as structural inheritence.

## Module

The module syntax has a number of important helpers. But most importantly, the module is the only set of helpers that get guarantees about *when* they run. All other define helpers are run in arbitrary order (often lazily).