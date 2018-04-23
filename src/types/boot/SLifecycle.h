#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** SLifecycle
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class SLifecycle abstract
		: public Aspect
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::types::SLifecycle, "types.life", InstanceAspectManager);

	public:
	};

}}
