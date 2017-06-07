#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Provider
	******************************************************************************/

	class Provider abstract
		: public Feature
	{
	};

	/******************************************************************************
	** IProviderManager
	******************************************************************************/

	class IProviderManager abstract
		: public IFeatureManager
	{
	public:
		virtual ~IProviderManager() = default;

		//
		// IFeatureManager
		//
		inline virtual FeatureKind featureKind() const override { return FeatureKind::Provider; }

		//
		// IProviderManager
		//
		virtual bool hasProvider(TypeId tid) = 0;
		virtual Provider* getProvider(TypeId tid) = 0;
		virtual void releaseProvider(Provider* provider) = 0;
	};

	/******************************************************************************
	** IIndexedProviderManager
	******************************************************************************/

	class IIndexedProviderManager abstract
	{
	public:
		virtual ~IIndexedProviderManager() = default;

		//
		// IIndexedProviderManager
		//
		virtual TypeId index(std::string const&) const = 0;
	};

	/******************************************************************************
	** ITaggedProviderManager
	******************************************************************************/

	class ITaggedProviderManager abstract
	{
	public:
		virtual ~ITaggedProviderManager() = default;

		//
		// ITaggedProviderManager
		//
		virtual std::set<TypeId> filter(std::string const&) const = 0;
	};
	
}}

#define CRAFT_PROVIDER_DECLARE(x, name, manager) \
	static ::craft::types::Feature::_static_init __si; \
protected: \
	typedef typename manager < x > T_Manager; \
private: \
	friend T_Manager; \
	friend class ::craft::types::Types; \
public: \
	static constexpr auto craft_c_featureKind = ::craft::types::FeatureKind::Provider; \
	static constexpr auto craft_c_featureName = name; \
	static inline ::craft::types::FeatureId craft_s_featureId() { return x::__si.id; } \
public: \
	virtual ::craft::types::IFeatureManager* craft_featureManager() const override \
	{ return ::craft::types::system().getManager(x::craft_s_featureId()); } \
private:

#define CRAFT_PROVIDER_DEFINE(x) \
	::craft::types::Feature::_static_init x::__si([](){ ::craft::types::system().getManager< x >(); });
