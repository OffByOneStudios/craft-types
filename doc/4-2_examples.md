
## Unique Pointers

### As Arguments

If a library expects a `std::unique_ptr` as an argument then it probably means the library wants to take ownership of that memory. This is a problem because the owner is going to be expecting to control that memory and be responsible for it's deallocation.

Unfortunetly this means we have to construct some memory for the object.

### Members

