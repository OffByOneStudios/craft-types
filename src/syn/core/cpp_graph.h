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
	** PCppDefine (NReference)
	******************************************************************************/

	struct PCppDefine final
	{
    public:
        CppDefine* define;
	};

}}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
