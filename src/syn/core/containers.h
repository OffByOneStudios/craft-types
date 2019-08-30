
#pragma once
#include "syn/syn.h"

/* These are hacky containers for temporary use, until we can replace them with better ones.
*/

namespace syn {
namespace core
{
	/******************************************************************************
	** prelude
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED extern Abstract AbstractContainer;

	/******************************************************************************
	** methods
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED extern Multimethod<> count;

	/******************************************************************************
	** String
	******************************************************************************/

    // defined in default_types_cpp.cpp
    typedef std::string String;

	/******************************************************************************
	** Vector
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED extern Abstract AbstractVector;

    typedef std::vector<instance<>> Vector;

    typedef std::vector<uint8_t> ByteVector;


	/******************************************************************************
	** Dictionary
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED extern Abstract AbstractDictionary;

    typedef std::map<instance<>, instance<>> Dictionary;
    
    typedef std::map<String, instance<>> StringDictionary;
    typedef std::map<syn::Symbol, instance<>> SymbolDictionary;

	/******************************************************************************
	** Set
	******************************************************************************/

	CULTLANG_SYNDICATE_EXPORTED extern Abstract AbstractSet;

    typedef std::set<instance<>> Set;
}}
