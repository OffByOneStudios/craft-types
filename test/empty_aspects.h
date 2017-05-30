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


template<typename T>
void describe_no_empty_aspects(craft::instance<T> &_)
{
	bandit::it(".hasFeature<SEmptyTestFactory>() is false", [&]()
	{
		AssertThat(_.hasFeature<SEmptyTestFactory>(), snowhouse::IsFalse());
	});
	bandit::it(".hasFeature<SEmptyTestInstance>() is false", [&]()
	{
		AssertThat(_.hasFeature<SEmptyTestInstance>(), snowhouse::IsFalse());
	});
	bandit::it(".getFeature<SEmptyTestFactory>() is nullptr", [&]()
	{
		AssertThat(_.getFeature<SEmptyTestFactory>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<SEmptyTestInstance>() is nullptr", [&]()
	{
		AssertThat(_.getFeature<SEmptyTestInstance>(), snowhouse::IsNull());
	});
}
