The new c++ define system is designed to be macro free and works as follows.

See [this for a working example](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwgQCeAB0wB9AsSaFBG7BwAMAQRu2WTALaZBephNWCdLBxwB2PjtVUNUCTGc9Zgj1LV0DJ1dVABVLBzDVJSZBQVUAEUwAMwVMAAlMBgNiDLDA4NtMvREAIwY8ZBBazNCFAlUi1FQNBszA/JH/ELCIqJjMOO19TCSFlLj81QA3VDx0dOnQwRMRZH7CkpZMbvUgm8yEzCwi7d30VQgAKiKASggL0oVKqYGpaNKyKxcABsP0mdnuYQBVyB1Q0mnB2FUAGUCCIikU4fZDj0XoYin5ZA0EaEkZgIC9yT9VCBVGSWPSWEz6qzyRAcXjfiNbhNieNCTdZtEmLE0Y9VqkDo0wtlcqoEDlDM9Sjd6jc9MQ8FtpZJqeFls8suriKodG4OABWHi0B0TSmmx4W5BW1QsYaOrgu8XEh6RKUy%2BLLeWaSzeAjSjo2u3Q8JuAgQVRYJSPCCaEAgJGEPCoTmqLlu4MzUPzRZylwLaMQ2Px5A%2B4bJiLHCAAOh7sPLRKVoWabQ6XQroRWImctx42yYygWGk2gjwAC9MKgihAOwQ0RiINYfmXXcu1xut7bBFyAq7RnUb0HBym5sbZZG66lSGbEh/sE42ousibDseyKpkxzEKc/SPJqxTav2EpVq%2BEY/skGI3BBUHfkYWpXG%2BqFrKQ7rvskxzoHmKxMABhh4EUxhouqgiwZc1xgpYeZGgu6QQjqdzjqoCjtFcTYEAmmYMNmKR5gWonFrCyYyUWLAbKk0lwSwhbFoSYwPghD7wgZtgAPRGdicYsOg87FtcdiYAAHhExDKT4LBqSxaIxvy%2BKPuJ2Zeb8ZkChAOqOi6EBMCIRCsted49KyXaDKgKn2tp975H2VKGSZqjYA5IJOAwBTqTZtiYWcqgAGJDFi/h8U%2BfSqC0TCgrFGaiABjVMKu4q6Zljh1u4ngLC5tWtZK8weTxxJldByzMfBmhVagNVTU%2Bur8cczbeL4bkLUtK2YopWn9jpt61SKA6%2BcsEAuXmMG4axi3Vex%2BbqZpJa3SA93FWi%2B0vUd7IhXw9r5OFkVJYYMWmoYCVDMlqWhOMGVTAO2UAPItFQmDlbSDgqnkS1o6NDjDu0nQYXGoktp9uNaITMYA4%2B%2BqGsaY71Sw/RNS1NzoO1DALE13V6WdhlXQYECE7tGmySWkuvZc73BaKoUg2DUWQzO0Ow0lS6qClJ1pcjhkOA19kmJ4BCGMcQWwvCdWZMQmC4k522uV9c0Pb9QxExCUvvV2/naz1F1SD8jDSPaUikK5UjWFHqDSNGvD8N4ojiIuXCyLQUcENIcdhxASD4Pi5CUAwCCCHQpDEBXdBhwwEdRzHcekAnUj5LRLx2QAHFCqjOIIWwtlstAAJxdrQfdbHkPd9wPQ/bGPE9Qjneeh2HADWID2rQXZcKPO%2Bj7IXABKPo8BPal/h1IAAsTdr630hR5X1ikLnseh6QcCwEgogEM0BBS4QDQFEPA/NiBVyeIQEgVcADupg9DSGzvXRu0cH5tzkLQVQsDCAIFULPfug9h5L0nqvD%2BBckAgL0GAkEQCqE0IgUPPQehDAj1HoYSehhZ6kBKBJEElcIAtAfi0BQzUdBIKjiA1wHM0YsAYOIj%2BpAsDOFYMAfmD98COzOIaNwD97LY0ipIKQ2dyAc0qA/doLRTDEB0JoDARiTEmDwM4CR9dVEoH4PwRgeAWiV0gGHVAegZaVykEZMiHjk68HmMcVQRk0ZcGfmnCQddr6RzQYotuBD57EPHpPD4uBoHWkwV%2BOxoDwHqEzrQJkSc%2BC8DIfnTeIAb7dy7AEawzTrBQgCFwaw1guA3xvrIKE1877pJbm3Z%2BIBX7vwadfBJYz45PzfmvI8pAtj8KUk0oAA%3D) of the template / define system.

### Define Libraries

There are different kinds of defines that can be performed. Technically any define can do any change it wants to the system graph. However we group these into **Define Libraries** to provide ease of use to the user, there is a helper `syn::DefineHelper` which takes a define library as it's second argument.

By default however the `syn::Define` template automatically chooses an appropriate library from the thing it is passed as the first argument:

* If the first argument is `void` (the default if `<>` is used) then it uses the base define library only. This is the one intended for entering raw information into the graph.
* If the first argument is a special syndicate object type then it will choose the appropriate definition library. These are often recursive (e.g. the `syn::Method<Signature>` object is a `syn::Define<syn::Method<Signature>>` ).
* If the first argument is some other type then it uses the type define library which is meant for defining types and objects.
  * The `syn::Define` object actually runs some special rules to determine what kind of node to place in the type graph at compile time (as some, like classes with virtual inheritance, require some special information) as nodes for all types are added first.

### Define

Define is used primarily for wrapping existing C++ objects in some way (or entering arbitrary information). Other more specific define types are always preferred for the compile time power they provide.

```c++
// enter arbitrary information
extern syn::Define<> PutInInformation;
```

All defines require a declaration 

```c++
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
class MyClass
{
  public:
    static syn::Define<MyClass> Definition;
    
  /* .. rest of my class ... */
};

// .cpp
decltype(MyClass::Definition) MyClass::Definition(
    [](auto _) {
        /* .. define description .. */
    });
```

#### Adding External Types

Adding external types requires a bit more rigamarol

```c++
// their header
class TheirClass
{
  /* .. their class ... */
};

// my header
namespace syn
{
    template<>
    struct type_define<TheirClass>
    {
        static syn::Define<TheirClass> Definition;
    };
}

// .cpp
decltype(syn::type_define<TheirClass>::Definition) syn::type_define<TheirClass>::Definition(
    [](auto _) {
        /* .. define description .. */
    });
```

The portion in "my header" is not strictly speaking necessary and can be replaced with:

```c++
extern syn::Define<TheirClass> TheirClassDefinition; // probably do NOT do this
```

However this prevents the runtime system's C++ helper library from functioning correctly (as the templates can not find the Definition for the `TheirClass` type at compile time). Which for some use cases may be *fine* but by convention the `syn::type_define` template should always be updated if the `Definition` is not placed in the class itself.

### Module

Modules are a special object that can be run before and after other initializers. In general each DLL should have a statically defined module that will determine the initialization policy of the static defines.

Modules can also be created dynamically, however it is strongly recommended that users inherit from the `syn::core::ModuleBase` class (it's in the runtime system! assuming the runtime system is compiled to run it's static initializers on load) and change some of it's behavior, rather than attempting to dynamically initialize `syn::Module`.

### Method

### Object

### Concept

