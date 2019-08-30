#pragma once
#include "syn/syn.h"

/* See section 2.3 of the manual.
*/

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace syn {
namespace core
{
	/******************************************************************************
	** NDispatcher (typenode NReference)
	******************************************************************************/

	struct NDispatcher final
	{
    public:
        void* dispatcher_state;
	};

	/******************************************************************************
	** EUsingDispatcherFunction (typenode NEmpty)
	******************************************************************************/

	struct EUsingDispatcherFunction final
	{
	private:
		// Unused
		uintptr_t _reserved;
	};
	
	/******************************************************************************
	** PCompositionalCast (typenode NStruct)
	******************************************************************************/

	// Placed on an IsA edge to describe how it can be casted between.
	struct PCompositionalCast final
	{
	public:
		ptrdiff_t offset;
	};

}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
