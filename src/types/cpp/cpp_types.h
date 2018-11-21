#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type_Node_CppFunction
	******************************************************************************/

	struct Type_Node_CppFunction final
	{
	private:
        typedef void (*_fnptr)();

		_fnptr _fn;
	};

	/******************************************************************************
	** Type_Property_CppIdentifier
	******************************************************************************/

    // The non-mangled fully qualified name
	struct Type_Property_CppIdentifier final
	{
		char const* cpp_identifier;
	};
    
	/******************************************************************************
	** Type_Property_CppStaticDescription
	******************************************************************************/

    // The static description object that was theorhetically responsible for this node
	struct Type_Property_CppStaticDescription final
	{
		cpp::static_desc* desc;
	};
}}
