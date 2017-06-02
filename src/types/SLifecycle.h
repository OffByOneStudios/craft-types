#pragma once
#include "common.h"
#include "core.h"
namespace craft {

	/******************************************************************************
	** SLifecycle
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class SLifecycle abstract
		: public Aspect
	{
		CRAFT_TYPE_EXPORTED CRAFT_ASPECT_DECLARE(SLifecycle, "types.life", Instance);

	public:
	};

}
