#pragma once
#include "syn/syn.h"

/* This file fixes up the system by declaring the system types as external cpp types.

	See the relevant .cpp files for the DEFINEs

*/

/******************************************************************************
** /types/system/store.h
******************************************************************************/

#include "syn/system/TypeStore.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::TypeStore);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::TypeId);

/******************************************************************************
** /types/system/basic_types.h
******************************************************************************/

#include "syn/system/basic_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Property_SystemGraphKind);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Property_NamespaceIdentifier);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Property_LocalIdentifier);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Node_AbstractType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Edge_IsA);

/******************************************************************************
** /types/system/native_types.h
******************************************************************************/

#include "syn/system/native_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Node_BitsType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Node_StructuralType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Property_NativeSize);

/******************************************************************************
** /types/system/advanced_types.h
******************************************************************************/

#include "syn/system/advanced_types.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Node_Multimethod);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Edge_MethodImplementation);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Node_GenericType);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::Type_Edge_GenericInstantiation);

/******************************************************************************
** /types/system/expression/expression.h
******************************************************************************/

#include "syn/system/expression/expression.h"

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionAny);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionVoid);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionBottom);

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionConcrete);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionArrow);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionTuple);

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, syn::ExpressionStore);

/******************************************************************************
** /types/system/expression/queries.h
******************************************************************************/

#include "syn/system/expression/queries.h"

namespace craft {
namespace types
{
	CRAFT_MULTIMETHOD_DECLARE(isSubtypeMethod, SimpleDispatcher);
}}
