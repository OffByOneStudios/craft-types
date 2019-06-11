#pragma once
#include "../common.h"
#include "../core.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
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

    
	/******************************************************************************
	** Type_Edge_CppLegacyImplements
	******************************************************************************/

    // The static description object that was theorhetically responsible for this node
	struct Type_Edge_CppLegacyImplements final
	{
		void* implementation;
	};

}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Node_CppFunction);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_CppIdentifier);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Property_CppStaticDescription);
CRAFT_TYPE_DECLARE(CRAFT_TYPES_EXPORTED, craft::types::Type_Edge_CppLegacyImplements);