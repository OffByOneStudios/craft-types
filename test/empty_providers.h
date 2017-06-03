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

	virtual std::string provider_index() = 0;
};

class PEmptyTestSingletonTagged abstract
	: public craft::Provider
{
	CRAFT_PROVIDER_DECLARE(PEmptyTestSingletonTagged, "types.test.empty.singleton-tagged", TaggedSingleton);

	virtual std::vector<std::string> provider_tags() = 0;
};

template<typename T>
void describe_no_empty_providers(craft::instance<T> &_)
{
	bandit::it(".hasFeature<PEmptyTestSingleton>() is false", [&]()
	{
		AssertThat(_.template hasFeature<PEmptyTestSingleton>(), snowhouse::IsFalse());
	});
	bandit::it(".hasFeature<PEmptyTestSingletonNamed>() is false", [&]()
	{
		AssertThat(_.template hasFeature<PEmptyTestSingletonNamed>(), snowhouse::IsFalse());
	});
	bandit::it(".hasFeature<PEmptyTestSingletonTagged>() is false", [&]()
	{
		AssertThat(_.template hasFeature<PEmptyTestSingletonTagged>(), snowhouse::IsFalse());
	});
	bandit::it(".getFeature<PEmptyTestSingleton>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingleton>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<PEmptyTestSingleton>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingletonNamed>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<PEmptyTestSingleton>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingletonTagged>(), snowhouse::IsNull());
	});
}
