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
	bool isSubtype(IExpression* left, IExpression* right);
}}
