#pragma once
#include "../common.h"
#include "../core.h"

/* This file fixes up the system by declaring the system types as external cpp types.

	See the relevant .cpp files for the DEFINEs

*/


/******************************************************************************
** /types/system/expression/expression.h
******************************************************************************/

#include "types/system/expression/expression.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, ::craft::types::ExpressionSpecial);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, ::craft::types::ExpressionConcrete);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, ::craft::types::ExpressionArrow);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, ::craft::types::ExpressionTuple);

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, ::craft::types::ExpressionStore);

/******************************************************************************
** /types/system/expression/queries.h
******************************************************************************/

#include "types/system/expression/queries.h"

namespace craft {
namespace types
{
	CRAFT_MULTIMETHOD_DECLARE(isSubtypeMethod, SimpleDispatcher);
}}
