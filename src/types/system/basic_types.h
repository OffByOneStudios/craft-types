#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type_Property_SystemGraphKind
	******************************************************************************/

	struct Type_Property_SystemGraphKind final
	{
		graph::GraphKind kind;
	};

	/******************************************************************************
	** Type_Property_NamespaceIdentifier
	******************************************************************************/

	struct Type_Property_NamespaceIdentifier final
	{
		char const* namespace_identifier;
	};

	/******************************************************************************
	** Type_Property_LocalIdentifier
	******************************************************************************/

	struct Type_Property_LocalIdentifier final
	{
		char const* local_identifier;
	};

	/******************************************************************************
	** Type_Node_AbstractType
	******************************************************************************/

	struct Type_Node_AbstractType final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** Type_Edge_IsA
	******************************************************************************/

	/* This represents the simple abstract type hierarchy.

	In so much as all complex types can be viewed as abstract types, the `is-a` edges are
	responsible for managing the abstract type hierarchy. Generally this will follow the "physical"
	type hierarchy but is not necessarly guaranteed to, for example, structural but not abstract
	inheritence of a physical type (like a primitive type).
	*/
	struct Type_Edge_IsA final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
}}
