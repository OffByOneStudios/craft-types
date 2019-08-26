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
	** NDispatcher (NReference)
	******************************************************************************/

	struct NDispatcher final
	{
    public:
        void* dispatcher;
	};

}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
