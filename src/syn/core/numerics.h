
#pragma once
#include "syn/syn.h"

/* These are the core numeric functions and types of the system.
*/

namespace syn {
namespace core
{

	/******************************************************************************
	** abstract hierarchy
	******************************************************************************/

    // { Signed, Unsigned } -> Integral -> Numeric
    // Floating -> Numeric

	CULTLANG_SYNDICATE_EXPORTED extern Abstract Numeric;
	CULTLANG_SYNDICATE_EXPORTED extern Abstract Integral;
	CULTLANG_SYNDICATE_EXPORTED extern Abstract Signed;
	CULTLANG_SYNDICATE_EXPORTED extern Abstract Unsigned;
	CULTLANG_SYNDICATE_EXPORTED extern Abstract Floating;


	/******************************************************************************
	** methods
	******************************************************************************/

	// Computes the addition of two numbers, is the symbol `+` in many programming languages.
	// Arguments:
	//     - 0 (required) numeric to add
	//     - 1 (required) other numeric to add
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> add;

	// Computes the subtraction of two numbers, is the symbol `+` in many programming languages.
	// Arguments:
	//     - 0 (required) numeric to sub from
	//     - 1 (required) numeric to sub
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> sub;

	// Computes the multiplication of two numbers, is the symbol `*` in many programming languages.
	// Arguments:
	//     - 0 (required) numeric to multiply
	//     - 1 (required) other numeric to multiply
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> mul;

	// Computes the division of two numbers, is the symbol `/` in many programming languages.
	// Arguments:
	//     - 0 (required) numeric to devide
	//     - 1 (required) numeric to devide by
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> div;
}}
