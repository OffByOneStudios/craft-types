#pragma once
#include "../common.h"
#include "../core.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
namespace craft {
namespace types
{
	/******************************************************************************
	** Type_Node_Multimethod
	******************************************************************************/

	struct Type_Node_Multimethod final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** Type_Edge_MethodImplementation
	******************************************************************************/

	/* This describes a dispatch relationship between a method and an executable function.

    Can be used for multi-methods or "normal" virtual methods. The dispatch type should be a tuple
    in the general case, but can be a single type in the case of single dispatch situations. If
    this edge involves a third node it is likely some sort of qualifier (e.g. around, before, etc.)
	*/
	struct Type_Edge_MethodImplementation final
	{
        ExpressionStore dispatch_type;
	};

	/******************************************************************************
	** Type_Node_GenericType
	******************************************************************************/

	/* This is a generic type.
	
	The type described *by* this node is a template type that describes all concrete realizations
	of this type in general terms. Generally, instantiating this will create a concrete type.
	
	A node of this type node may be used by other special generic features to wrap some sort of
	dispatcher or generic implementation around any nstance of this generic. For example: 

	* A is a concrete type.
	* G is a generic type with one argument.
	* S is a generic wrapper type.
	* G_0A is the concrete instantiation of the generic type G on A.
	* S_0G is the concrete instantiation of the generic type S on G.

	A user of a type system (e.g. a programming language) might say `G<A>` to get G_0A, but could
	also say `G<Any>` to get a S_0G, this would be a feature of the language, as the naieve
	interpretation of this request would be G_0ANY.
	*/
	struct Type_Node_GenericType final
	{
		uint8_t generic_parameter_count;
	};

	/******************************************************************************
	** Type_Edge_GenericInstantiation
	******************************************************************************/

	/* This describes that a concrete type is an instantiation of a generic type. */
	struct Type_Edge_GenericInstantiation final
	{
	private:
		// Unused
		size_t _reserved;
	};
}}
#pragma clang diagnostic pop
