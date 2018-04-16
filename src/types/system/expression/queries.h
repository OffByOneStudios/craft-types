#pragma once
#include "../../common.h"
#include "../../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Subtype
	******************************************************************************/

	// Checks to see if left is a subtype of right
	CRAFT_TYPES_EXPORTED bool isSubtype(IExpression const* left, IExpression const* right);
}}
