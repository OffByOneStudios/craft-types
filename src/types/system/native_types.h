#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type_Node_BitsType
	******************************************************************************/

	struct Type_Node_BitsType final
	{
		uint32_t bits;
		uint32_t _reserved;
	};

	/******************************************************************************
	** Type_Node_StructuralType
	******************************************************************************/

	// This is a plain old concrete structural type.
	struct Type_Node_StructuralType final
	{
		size_t bytes;
	};

	/******************************************************************************
	** Type_Property_NativeSize
	******************************************************************************/

	struct Type_Property_NativeSize final
	{
        // TODO: Singular
		size_t bytes;
	};
}}
