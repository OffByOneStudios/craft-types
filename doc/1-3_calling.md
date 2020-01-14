# Calling

Here we describe the runtime calling structures. Notably dispatchers and call conversion.

## Call Conversion

The fundamental issue here is that the arguments a function is dispatching off of are independent of the shape of the arguments it is receiving. This is related to the issue of calling conventions and memory ownership, among others.

For example a C function might take a pointer or might take a value. An instance might be able to be unwrapped to those types. On the other hand a function might take instances, but be handed a pointer to the value. In this case an instance must be constructed before being handed to the function.

In general instances are the gold standard of storing all relevant information (though their performance might not be great, they are meant as a compatibility structure first anyway).