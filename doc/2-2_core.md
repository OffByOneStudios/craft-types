
### Core

Hence the first collection of types we describe are meant to be simple as possible, while being expressive enough to describe themselves. We place these types in `syn::core` which is the core graph of the syndicate system. We also place these types in the runtime namespace `/Syndicate/Core/` (which is later forwarded to `/Syndicate/`)

#### Abstract

First we must describe an abstract type. These are simply nodes which we can attach symbols to later. They are our first representation of a type, and the primary representation of types that can be filled in later.

In the future they can have complex rules associated with what can and can be considered a part of the abstract type. At the moment we will have to be careful to ensure we do not violate any rules we rely on because of these abstracts.

For now we use a simple rule, if a type can find a path between itself and the target abstract node then it fits.

* `syn::core::NAbstract` (`/Syndicate/Core/Abstract`) an abstract type. Empty.
* `syn::core::EIsA` (`/Syndicate/Core/IsA`) An edge which points from a type to the other types it can be used in place of. Empty.

We also generate some system abstracts and add them to the graph:

* `syn::core::MetaType` (`/Syndicate/Core/MetaType`) Abstract for types used as types in the type graph.
* `syn::core::MetaNode` (`/Syndicate/Core/MetaNode` ) Abstract for types that represent nodes in the type graph.
  * `syn::core::MetaEdge` (`/Syndicate/Core/MetaEdge`)
  * `syn::core::MetaLabel` (`/Syndicate/Core/MetaLabel`)
  * `syn::core::MetaProp` (`/Syndicate/Core/MetaProp`)

#### C Structure

Now we can support C style structs. This involves a node that represents the structure, this node is mostly empty besides the size of the structure. And then a property for the in memory size of a structure.

* `syn::core::NStruct` (`/Syndicate/Core/Struct`) a node for a struct type (that is any type that takes up a chunk of memory)

#### Function