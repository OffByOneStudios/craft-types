#pragma once
#include "syn/syn.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Subtype
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED bool isSubtype(Graph::Node* left, Graph::Node* right);

	// Checks to see if left is a subtype of right
	CULTLANG_SYNDICATE_EXPORTED bool isSubtype(IExpression const* left, IExpression const* right);
}}
