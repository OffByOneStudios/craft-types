The C++ define system is designed to allow for the definition (and related declaration) of graph elements from within C++. The primary goal of this system is to allow flexibility and power, it was also designed to be macro free (which causes some annoyances with extra boilerplate).

See [this for a working simple example](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwgQCeAB0wB9AsSaFBG7BwAMAQRu2WTALaZBephNWCdLBxwB2PjtVUNUCTGc9Zgj1LV0DJ1dVABVLBzDVJSZBQVUAEUwAMwVMAAlMBgNiDLDA4NtMvREAIwY8ZBBazNCFAlUi1FQNBszA/JH/ELCIqJjMOO19TCSFlLj81QA3VDx0dOnQwRMRZH7CkpZMbvUgm8yEzCwi7d30VQgAKiKASggL0oVKqYGpaNKyKxcABsP0mdnuYQBVyB1Q0mnB2FUAGUCCIikU4fZDj0XoYin5ZA0EaEkZgIC9yT9VCBVGSWPSWEz6qzyRAcXjfiNbhNieNCTdZtEmLE0Y9VqkDo0wtlcqoEDlDM9Sjd6jc9MQ8FtpZJqeFls8suriKodG4OABWHi0B0TSmmx4W5BW1QsYaOrgu8XEh6RKUy%2BLLeWaSzeAjSjo2u3Q8JuAgQVRYJSPCCaEAgJGEPCoTmqLlu4MzUPzRZylwLaMQ2Px5A%2B4bJiLHCAAOh7sPLRKVoWabQ6XQroRWImctx42yYygWGk2gjwAC9MKgihAOwQ0RiINYfmXXcu1xut7bBFyAq7RnUb0HBym5sbZZG66lSGbEh/sE42ousibDseyKpkxzEKc/SPJqxTav2EpVq%2BEY/skGI3BBUHfkYWpXG%2BqFrKQ7rvskxzoHmKxMABhh4EUxhouqgiwZc1xgpYeZGgu6QQjqdzjqoCjtFcTYEAmmYMNmKR5gWonFrCyYyUWLAbKk0lwSwhbFoSYwPghD7wgZtgAPRGdicYsOg87FtcdiYAAHhExDKT4LBqSxaIxvy%2BKPuJ2Zeb8ZkChAOqOi6EBMCIRCsted49KyXaDKgKn2tp975H2VKGSZqjYA5IJOAwBTqTZtiYWcqgAGJDFi/h8U%2BfSqC0TCgrFGaiABjVMKu4q6Zljh1u4ngLC5tWtZK8weTxxJldByzMfBmhVagNVTU%2Bur8cczbeL4bkLUtK2YopWn9jpt61SKA6%2BcsEAuXmMG4axi3Vex%2BbqZpJa3SA93FWi%2B0vUd7IhXw9r5OFkVJYYMWmoYCVDMlqWhOMGVTAO2UAPItFQmDlbSDgqnkS1o6NDjDu0nQYXGoktp9uNaITMYA4%2B%2BqGsaY71Sw/RNS1NzoO1DALE13V6WdhlXQYECE7tGmySWkuvZc73BaKoUg2DUWQzO0Ow0lS6qClJ1pcjhkOA19kmJ4BCGMcQWwvCdWZMQmC4k522uV9c0Pb9QxExCUvvV2/naz1F1SD8jDSPaUikK5UjWFHqDSNGvD8N4ojiIuXCyLQUcENIcdhxASD4Pi5CUAwCCCHQpDEBXdBhwwEdRzHcekAnUj5LRLx2QAHFCqjOIIWwtlstAAJxdrQfdbHkPd9wPQ/bGPE9Qjneeh2HADWID2rQXZcKPO%2Bj7IXABKPo8BPal/h1IAAsTdr630hR5X1ikLnseh6QcCwEgogEM0BBS4QDQFEPA/NiBVyeIQEgVcADupg9DSGzvXRu0cH5tzkLQVQsDCAIFULPfug9h5L0nqvD%2BBckAgL0GAkEQCqE0IgUPPQehDAj1HoYSehhZ6kBKBJEElcIAtAfi0BQzUdBIKjiA1wHM0YsAYOIj%2BpAsDOFYMAfmD98COzOIaNwD97LY0ipIKQ2dyAc0qA/doLRTDEB0JoDARiTEmDwM4CR9dVEoH4PwRgeAWiV0gGHVAegZaVykEZMiHjk68HmMcVQRk0ZcGfmnCQddr6RzQYotuBD57EPHpPD4uBoHWkwV%2BOxoDwHqEzrQJkSc%2BC8DIfnTeIAb7dy7AEawzTrBQgCFwaw1guA3xvrIKE1877pJbm3Z%2BIBX7vwadfBJYz45PzfmvI8pAtj8KUk0oAA%3D) of the template / define system to understand it's design.

### Define Libraries

There are different kinds of defines that can be performed. Technically any define can do any change it wants to the system graph. However we group helper actions into **Define Libraries** to provide ease of use to the user, the core of the define system is the `syn::DefineHelper` which takes a define library as it's second (template) argument (the first being the compile time object being manipulated).

By default however the `syn::Define` template automatically chooses an appropriate library from the thing it is passed as the first argument:

* If the first argument is `void` (the default if `<>` is used) then it uses the base define library only. This is the one intended for helping enter raw information into the graph.
* If the first argument is a special syndicate object type then it will choose the appropriate definition library. These are often recursive (e.g. the `syn::Method<TDispatcher>` object is a `syn::Define<syn::Method<TDispatcher>>`) this is often used to detect special objects.
* If the first argument is some other type then it uses the type define library which is meant for defining types and objects.
  * The `syn::Define` object actually runs some special rules to determine what kind of node to place in the type graph at compile time (as some, like classes with virtual inheritance, require some special information). This is required because nodes for all static types are added before the define helper function is ran.

### Define

Direct use of `syn::Define` (e.g. instead of through inheritence) is primarily intended for wrapping existing C++ objects in some way (or entering arbitrary information). Other more specific (e.g. inherited) define types are always preferred for the compile time power they provide.

```c++
// .h
// enter arbitrary information
extern syn::Define<> PutInInformation;
```

Informational defines don't - strictly speaking - require a declaration like above, but it is conventional.

```c++
// .cpp
// declare the static variable with the function to enter the information
decltype(PutInInformation) PutInInformation(
    [](auto _) {
        auto n = _.addNode<syn::core::NStruct>();
        /* ...add a type by hand... */
    });
```

There are multiple ways to add types to the system, they are covered in more depth in [section 3.3](3-3_cpp-object.md). For more explanation of the define syntax read [section 4.3](4-3_defines).

#### Adding Runtime System Aware Types

This is the simplest way to add new runtime system aware types, while keeping all C++ semantics intact. It is the easiest way to get started with the type system:

```c++
// .h
class MyClass
{
  public:
    // we add the definition within the declaration of the class
    // it is important for this to be static (effectively extern)
    // it is important for this to have the name `Definition`.
    static syn::Define<MyClass> Definition;
    
  /* .. rest of my class ... */
};

// .cpp
// we have to actually define the class features here
decltype(MyClass::Definition) MyClass::Definition(
    [](auto _) {
        /* .. define description .. */
    });
```

Note the declaration containing the definition object allows for access to syndicate features in C++. If it is not named correctly many C++ syndicate features will fail (most will cause compile errors).

#### Adding External Types

Adding external types requires a bit more work to add them to the runtime system.

```c++
// .h
// their header
class TheirClass
{
  /* .. their class ... */
};

// .h
// my header
namespace syn
{
    // update syndicate's machinery
    template<>
    struct type_define<TheirClass>
    {
        // it is important for this to be static (effectively extern)
        // it is important for this to have the name `Definition`.
        static syn::Define<TheirClass> Definition;
    };
}

// .cpp
decltype(syn::type_define<TheirClass>::Definition) syn::type_define<TheirClass>::Definition(
    [](auto _) {
        /* .. define description .. */
    });
```

The portion in "my header" is not strictly speaking necessary. However it is required for access to syndicate features in C++. What we are doing here is updating syndicate's internal define template (used to access types at compile time) by hand.

It could be replaced with an external define block (for example):

```c++
extern syn::Define<TheirClass> TheirClassDefinition; // probably do NOT do this
```

### Module

Modules are a special object that can be run before and after other initializers. In general each DLL should have a statically defined module - this can determine the initialization policy of the static defines, amoung other configuration information - but is generally meant for identification of the DLL and it's contents.

Module initalizers are generally used to add additional information - rather than using `syn::Define<void>` - for small modules, however they can also be mixed with the defines (and control which are ran!).

Modules can also be created dynamically, however it is strongly recommended that users inherit from the `syn::core::ModuleBase` class (it's in the runtime system! assuming the runtime system is compiled to run it's static initializers on load) and change some of it's behavior, rather than attempting to dynamically initialize `syn::Module` (which expects to be a statically initalized define helper).

### Method

Methods are objects that contain a collection of functions (generally including a dispatcher of some sort). Plain functions are generally added in void defines or module defines (or any define!), but these don't have dispatch information. Our primary concern 


```c++
// .h
// thisthe argument here is the C++ compile time dispatcher, not the runtime dispatcher, not currently supported
extern syn::Method<> MyMethod;

// .cpp
// declare the static variable with the function to enter the information
decltype(MyMethod) MyMethod(
    [](auto _) {
        _.method(&someFunction)
            ...;
        /* .. add more methods .. */
    });
```


### Object

### Concept

