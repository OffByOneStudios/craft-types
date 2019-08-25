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
	** NEmpty
	******************************************************************************/

	struct NEmpty final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** NAbstract (typenode NEmpty)
	******************************************************************************/

	struct NAbstract final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** EIsA (typenode NEmpty)
	******************************************************************************/

	struct EIsA final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};

	/******************************************************************************
	** NBits (typenode NStruct)
	******************************************************************************/

	struct NBits final
	{
	public:
		uint64_t bits;
	};
	
	/******************************************************************************
	** NStruct (typenode NStruct)
	******************************************************************************/

	struct NStruct final
	{
	public:
		uint64_t bytes;
	};
	
	/******************************************************************************
	** NReference (typenode NEmpty)
	******************************************************************************/

	struct NReference final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
	
	/******************************************************************************
	** NSignature (typenode NEmpty)
	******************************************************************************/

	struct NSignature final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
	
	/******************************************************************************
	** NFunction (typenode NReference)
	******************************************************************************/

	struct NFunction final
	{
	public:
		void (*fptr)();
	};

}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
