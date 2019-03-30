#pragma once
#include "../common.h"
#include "../core.h"

/* This file fixes up the system by declaring the system types as external cpp types.

	See the relevant .cpp files for the DEFINEs

*/

/******************************************************************************
** /types/system/store.h
******************************************************************************/

#include "types/system/store.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::TypeStore);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::TypeId);

/******************************************************************************
** /types/system/basic_types.h
******************************************************************************/

#include "types/system/basic_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_SystemGraphKind);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_NamespaceIdentifier);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_LocalIdentifier);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_AbstractType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Edge_IsA);

/******************************************************************************
** /types/system/native_types.h
******************************************************************************/

#include "types/system/native_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_BitsType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_StructuralType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_NativeSize);

/******************************************************************************
** /types/system/advanced_types.h
******************************************************************************/

#include "types/system/advanced_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_Multimethod);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Edge_MethodImplementation);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_GenericType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Edge_GenericInstantiation);

/******************************************************************************
** /types/system/expression/expression.h
******************************************************************************/

#include "types/system/expression/expression.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionAny);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionVoid);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionBottom);

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionConcrete);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionArrow);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionTuple);

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::ExpressionStore);

/******************************************************************************
** /types/system/expression/queries.h
******************************************************************************/

#include "types/system/expression/queries.h"

namespace craft {
namespace types
{
	CRAFT_MULTIMETHOD_DECLARE(isSubtypeMethod, SimpleDispatcher);
}}
