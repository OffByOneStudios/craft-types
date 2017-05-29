#pragma once
#include "common.h"

class PEmptyTestSingleton abstract
	: public craft::Provider
{
	CRAFT_PROVIDER_DECLARE(PEmptyTestSingleton, "types.test.empty.singleton", Singleton);
};

class PEmptyTestSingletonNamed abstract
	: public craft::Provider
{
	CRAFT_PROVIDER_DECLARE(PEmptyTestSingletonNamed, "types.test.empty.singleton-named", NamedSingleton);
};

class PEmptyTestSingletonTagged abstract
	: public craft::Provider
{
	CRAFT_PROVIDER_DECLARE(PEmptyTestSingletonTagged, "types.test.empty.singleton-tagged", TaggedSingleton);
};
