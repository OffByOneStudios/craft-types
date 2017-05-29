
## Design

This type system is designed to provide reflection capabilities. This comes in two parts: a set of features for interacting with data and a default object system that provides these features in an easy to use fashion.

At the root of the system is a `Types` singleton which provides a jumping off point for most capabilities. We use auto incremented identifiers (like components) - `TypeId`s - to identify each type in the system. Instances of types come in a couple varieties depending on the features wanted. Features share an auto-generated `FeatureId`.

`Instance`s are fully featured structure of an instance: pointer to a concrete type and pointer to metadata (type id, GC, feature cache), (optionally) pointers to specific features. Instances can be null, featured instances may not have a feature while having an object.

The first interaction feature is the `Provider`. Because they can *optionally* take an instance they can be used to create new instances, often done as singletons.

The second interaction feature is the `Aspect`. Aspects describe a specific set of features for a specific instance.

The default object system provides a set of features (like macros and templates) that make it easier to provide default features: Garbage collection, reflection, marshalling, etc. The `Object` class is the virtual abstract base class, it should be inherited virtually. The object system also provides macros for initializing objects with custom features.

## Classes

### IProviderManager

Provider management interface.

### IAspectManager

Aspect management interface.

# OLD

We provide an `ObjectBase` class which provides a general interface for classes in our object system (namely `craft_type_id`, inline methods calling a type ECS, and some pointer math). There is a typed version (call Object) which templates off of the class inheriting from it and automatically provides `ObjectBase` as well as invoke the unique type id features. Object's should be inherited virtually.

We provide an `InterfaceBase` class which provides mechanisms for knowing which interfaces an instance for which types. There is a templated `Interface` class which is templated off of an abstract interface and also provides a management strategy for that interface, it also provides the interface id infrastructure. There are two types of interfaces, generic ones and templated ones. Templated interfaces should have an `I*Base` class which is generic (even if it's empty), this class will also be provided to the `Interface` template (Second parameter). A generic interface can also provided templated methods, but this is only recommended when one instance may actually provide implementations for many objects.

The resulting interfaces should be inherited from normally, each will have their own InterfaceBase implementations. Implementations will have to be added depending on the strategy each interface employs:

* `Singleton`: Simply create an instance of the class and add the instance's matching interface to the ECS at runtime.
* `Provider`: The instance set into the type ECS is a factory instance, taking the Object as part of the information required to choose the interface instance, this interface then behaves like a normal cast operation. Templates are provided for various ways of doing this.
* `Mediator`: This takes arbitrary information to determine which interface to choose. The mediator may also choose a specific object (although their interface will have to communicate it).

The Type ECS is our runtime type system. Entity allocation is done by a special singleton entity allocator which is shared between all the type ECS instances.
