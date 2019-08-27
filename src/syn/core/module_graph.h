#pragma once
#include "syn/syn.h"

/* See section 2.x of the manual.
*/

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace syn {
namespace core
{
	/******************************************************************************
	** NModule
	******************************************************************************/

	struct NModule final
	{
        ModuleBase* module;
	};

	/******************************************************************************
	** PModuleSymbol
	******************************************************************************/

	/* The symbol this type has in it's module
	 *
	 */
	struct PModuleSymbol final
	{
        Symbol symbol;
	};

	/******************************************************************************
	** EMemberOfModule
	******************************************************************************/

	struct EMemberOfModule final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif