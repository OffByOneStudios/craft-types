#pragma once
#include "syn/common.h"

class PEmptyTestSingleton abstract
	: public syn::Provider
{
	CRAFT_LEGACY_FEATURE_DECLARE(PEmptyTestSingleton, "types.test.empty.singleton", syn::SingletonProviderManager);
};

class PEmptyTestSingletonNamed abstract
	: public syn::Provider
{
	CRAFT_LEGACY_FEATURE_DECLARE(PEmptyTestSingletonNamed, "types.test.empty.singleton-named", syn::NamedSingletonProviderManager);

	virtual std::string provider_index() = 0;
};

class PEmptyTestSingletonTagged abstract
	: public syn::Provider
{
	CRAFT_LEGACY_FEATURE_DECLARE(PEmptyTestSingletonTagged, "types.test.empty.singleton-tagged", syn::TaggedSingletonProviderManager);

	virtual std::vector<std::string> provider_tags() = 0;
};

template<template<typename> class T>
inline void describe_for_empty_providers(std::string desc)
{
	bandit::describe(desc.c_str(), []() {
		bandit::describe("PEmptyTestSingleton", T<PEmptyTestSingleton>());
		bandit::describe("PEmptyTestSingletonNamed", T<PEmptyTestSingletonNamed>());
		bandit::describe("PEmptyTestSingletonTagged", T<PEmptyTestSingletonTagged>());
	});
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
	/*
	bandit::it(".getFeature<PEmptyTestSingleton>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingleton>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<PEmptyTestSingletonNamed>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingletonNamed>(), snowhouse::IsNull());
	});
	bandit::it(".getFeature<PEmptyTestSingletonTagged>() is nullptr", [&]()
	{
		AssertThat(_.template getFeature<PEmptyTestSingletonTagged>(), snowhouse::IsNull());
	});
	*/
}
