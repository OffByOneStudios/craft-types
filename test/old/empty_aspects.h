#pragma once
#include "syn/common.h"

class SEmptyTestFactory abstract
	: public syn::Aspect
{
	CRAFT_LEGACY_FEATURE_DECLARE(SEmptyTestFactory, "types.test.empty.factory", syn::FactoryAspectManager);
};

class SEmptyTestInstance abstract
	: public syn::Aspect
{
	CRAFT_LEGACY_FEATURE_DECLARE(SEmptyTestInstance, "types.test.empty.instance", syn::InstanceAspectManager);
};

template<template<typename> class T>
inline void describe_for_empty_aspects(std::string desc)
{
	bandit::describe(desc.c_str(), []() {
		bandit::describe("SEmptyTestFactory", T<SEmptyTestFactory>());
		bandit::describe("SEmptyTestInstance", T<SEmptyTestInstance>());
	});
};

template<typename T>
void describe_no_empty_aspects(craft::instance<T> &_)
{
	bandit::it(".hasFeature<SEmptyTestFactory>() is false", [&]()
	{
		AssertThat(_.template hasFeature<SEmptyTestFactory>(), snowhouse::IsFalse());
	});
	bandit::it(".hasFeature<SEmptyTestInstance>() is false", [&]()
	{
		AssertThat(_.template hasFeature<SEmptyTestInstance>(), snowhouse::IsFalse());
	});
	/*
	bandit::it(".getFeature<SEmptyTestFactory>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<SEmptyTestFactory>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<SEmptyTestInstance>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<SEmptyTestInstance>(), snowhouse::IsNull());
	});
	*/
}
