#pragma once
#include "../common.h"
#include "../core.h"

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
	private:
        ExpressionStore dispatch_type;
	};
}}
