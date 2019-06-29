#pragma once
#include "syn/syn.h"

/* See section 2.2 of the manual.
*/

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace syn {
namespace core
{
	/******************************************************************************
	** NAbstractType
	******************************************************************************/

	struct NAbstractType final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** EIsA
	******************************************************************************/

	struct EIsA final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
