# Syndicate

This is a runtime system primarily intended for the Cult language. It's also intended to provide dynamic runtime features to native code (specifically, but not exclusively, C++). It does both of these goals by providing an extensible self hosting type system built around an in memory graph (provided by [our C++ graph library](https://github.com/cultlang/graph)). From there we build a basic library of types which supports multi-methods, algebraic data types (also plain interfaces), dynamic modules (including hot reload), and dynamic stack frames (including a condition system and dynamic scoping).

**This library is intended for research and is currently under development. It is not stable and is not intended for production use.** This library is currently being refactored, there is a lot of dead cold and old documentation.

[Read the manual](https://github.com/cultlang/syndicate/tree/master/doc).