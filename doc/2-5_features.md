
### Objects

Here we add generic object features.

### Lifecycle

Here we add generic lifecycle features.

#### Meta Rules

The first set of generic features are those that manage the lifecycle of objects. For this we create some types in the runtime:

* `syn::core::LMetaRuleLifecycle` (`/Syndicate/Core/MetaRuleLifecycle`) this is a label for metatypes that effect the lifecycle of an object.

**Rule** an interpreter may only work with the memory of objects when it has a complete understanding of the object's type node's type, the object's type node's edge types, and the object's type node's property types that are in the label space `/Syndicate/Core/MetaRuleLifecycle`.

#### InitialMemory

This is a property which tells any interpreting system that this type begins initalization by copying the memory contained in this property.

* `syn::core::graph::PInitialMemory` (`/Syndicate/Core/InitialMemory`) a node for a struct type (that is any type that takes up a chunk of memory). The value is a pointer for the memory to initalize with.

This leads to a computed property:

* `PlainOldData`: A type is "plain old data"

#### Constructor

#### Destructor

