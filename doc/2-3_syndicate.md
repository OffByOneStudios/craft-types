Here we describe the advanced features that syndicate builds ontop the core, that we consider core implementation features (but are not strictly speaking required).

### Instance

instance

### Dispatcher

method, dispatcher

#### Methods (OLD)

We have descending levels of representation for subroutines.

A simple function node with a signature is a standalone function. If bound to a name it would only be called if types matched exactly.

A collection of function nodes with differing signatures that are all connected to a dispatch node is a form of method. If a name is bound to that dispatch node it would be a multi-method, and allow runtime dispatch. A property placed on it might *degrade* that feature into compile time dispatch only.

A collection of function nodes with signatures differing only in the first argument that are all connected to a dispatch node (with a property to constrain it so) is a form of method. If a qualified name is bound to it, it might be called an object method, or virtual function.
