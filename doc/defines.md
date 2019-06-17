The new c++ define system is designed to be macro free and works as follows.

See [this for a working example](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwgQCeAB0wB9AsSaFBG7BwAMAQRu2WTALaZBephNWCdLBxwB2PjtVUNUCTGc9Zgj1LV0DJ1dVABVLBzDVJSZBQVUAEUwAMwVMAAlMBgNiDLDA4NtMvREAIwY8ZBBazNCFAlUi1FQNBszA/JH/ELCIqJjMOO19TCSFlLj81QA3VDx0dOnQwRMRZH7CkpZMbvUgm8yEzCwi7d30VQgAKiKASggL0oVKqYGpaNKyKxcABsP0mdnuYQBVyB1Q0mnB2FUAGUCCIikU4fZDj0XoYin5ZA0EaEkZgIC9yT9VCBVGSWPSWEz6qzyRAcXjfiNbhNieNCTdZtEmLE0Y9VqkDo0wtlcqoEDlDM9Sjd6jc9MQ8FtpZJqeFls8suriKodG4OABWHi0B0TSmmx4W5BW1QsYaOrgu8XEh6RKUy%2BLLeWaSzeAjSjo2u3Q8JuAgQVRYJSPCCaEAgJGEPCoTmqLlu4MzUPzRZylwLaMQ2Px5A%2B4bJiLHCAAOh7sPLRKVoWabQ6XQroRWImctx42yYygWGk2gjwAC9MKgihAOwQ0RiINYfmXXcu1xut7bBFyAq7RnUb0HBym5sbZZG66lSGbEh/sE42ousibDseyKpkxzEKc/SPJqxTav2EpVq%2BEY/skGI3BBUHfkYWpXG%2BqFrKQ7rvskxzoHmKxMABhh4EUxhouqgiwZc1xgpYeZGgu6QQjqdzjqoCjtFcTYEAmmYMNmKR5gWonFrCyYyUWLAbKk0lwSwhbFoSYwPghD7wgZtgAPRGdicYsOg87FtcdiYAAHhExDKT4LBqSxaIxvy%2BKPuJ2Zeb8ZkChAOqOi6EBMCIRCsted49KyXaDKgKn2tp975H2VKGSZqjYA5IJOAwBTqTZtiYWcqgAGJDFi/h8U%2BfSqC0TCgrFGaiABjVMKu4q6Zljh1u4ngLC5tWtZK8weTxxJldByzMfBmhVagNVTU%2Bur8cczbeL4bkLUtK2YopWn9jpt61SKA6%2BcsEAuXmMG4axi3Vex%2BbqZpJa3SA93FWi%2B0vUd7IhXw9r5OFkVJYYMWmoYCVDMlqWhOMGVTAO2UAPItFQmDlbSDgqnkS1o6NDjDu0nQYXGoktp9uNaITMYA4%2B%2BqGsaY71Sw/RNS1NzoO1DALE13V6WdhlXQYECE7tGmySWkuvZc73BaKoUg2DUWQzO0Ow0lS6qClJ1pcjhkOA19kmJ4BCGMcQWwvCdWZMQmC4k522uV9c0Pb9QxExCUvvV2/naz1F1SD8jDSPaUikK5UjWFHqDSNGvD8N4ojiIuXCyLQUcENIcdhxASD4Pi5CUAwCCCHQpDEBXdBhwwEdRzHcekAnUj5LRLx2QAHFCqjOIIWwtlstAAJxdrQfdbHkPd9wPQ/bGPE9Qjneeh2HADWID2rQXZcKPO%2Bj7IXABKPo8BPal/h1IAAsTdr630hR5X1ikLnseh6QcCwEgogEM0BBS4QDQFEPA/NiBVyeIQEgVcADupg9DSGzvXRu0cH5tzkLQVQsDCAIFULPfug9h5L0nqvD%2BBckAgL0GAkEQCqE0IgUPPQehDAj1HoYSehhZ6kBKBJEElcIAtAfi0BQzUdBIKjiA1wHM0YsAYOIj%2BpAsDOFYMAfmD98COzOIaNwD97LY0ipIKQ2dyAc0qA/doLRTDEB0JoDARiTEmDwM4CR9dVEoH4PwRgeAWiV0gGHVAegZaVykEZMiHjk68HmMcVQRk0ZcGfmnCQddr6RzQYotuBD57EPHpPD4uBoHWkwV%2BOxoDwHqEzrQJkSc%2BC8DIfnTeIAb7dy7AEawzTrBQgCFwaw1guA3xvrIKE1877pJbm3Z%2BIBX7vwadfBJYz45PzfmvI8pAtj8KUk0oAA%3D) of the template / define system.

### Kinds

There are different kinds of defines that can be performed. Technically any define can do any change it wants to the system graph.

* `Extend`: Extend existing information.
* `Function`: A bare function, prefer multi-method.
* `MultiMethod`: A multimethod.
* `Class`: An object description (from plain old data to complex).
* `Concept`: A conceptual description abstract type. For both providers and aspects.

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