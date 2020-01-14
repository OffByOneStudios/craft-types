# Instance

The instance system is our runtime description of an object. It needs to be able to handle the following parts of the runtime management of the object:

* "memory": Where does the objects memory live?
* "concrete": What is the concrete type of the object (and hence it's memory layout)?
* "lifecycle": When will the object be deleted (and how)?
  * May also include thread information like locks.

Further the actual interpreter of these headers (for C++, defined later as the `instance` helper templates) need to address the following issues:

* Storing a reference to the header.
* "actual": The actual memory currently being manipulated (for example a sub object or a virtual dispatch table).
* "type": The type expression currently being represented (which should lead to an interpretation of actual).
* "ownership": The ownership state of the instance.
  * May also include thread information (e.g. to avoid locking if this thread owns it)

### `InstanceHeader`

This is the generic header into which any concrete object can be stored. It stores the following fields:

* `memory` A void* to the memory, might be packed directly after the object, or packed into the pointer itself.
* `concrete` The concrete type identifier (a pointer to a graph node).
* `lifecycle` The lifecycle flags (and counter).
* `manager` A more advanced manager for the object.
