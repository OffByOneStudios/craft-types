
#pragma once
#include "syn/syn.h"

/* These are the core conversion methods of the system.
*/

namespace syn {
namespace core
{

	/******************************************************************************
	** stringly
	******************************************************************************/

	// Takes a string and returns a type
	// Arguments:
	//     - 0 (required) The expected return type to parse to
	//     - 1 (required) The string to parse
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> parse;

	// Creates a string of only the value (never the type, never meta data) and perhaps state this should be suitable for parsing in many cases
	// Arguments:
	//     - 0 (required) value to convert to string
	//     - 1 (optional) printing control
	//         - if integral type: depth to print to in recursive cases
	// Names:
	//     glyphize? letterize?
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> value_string;
	
	// Creates a text description (likely a string) of the type, value, state and other metadata
	// Arguments:
	//     - 0 (required) value to print
	//     - 1 (optional) printing control
	//         - if integral type: depth to print to in recursive cases
	// Names:
	//     glyphize? letterize?
	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> description_text;
}}
