#pragma once
#include "common.h"

class SEmptyTestFactory abstract
	: public craft::Aspect
{
	CRAFT_ASPECT_DECLARE(SEmptyTestFactory, "types.test.empty.factory", Factory);
};

class SEmptyTestInstance abstract
	: public craft::Aspect
{
	CRAFT_ASPECT_DECLARE(SEmptyTestInstance, "types.test.empty.instance", Instance);
};
