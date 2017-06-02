#pragma once
#include "common.h"
#include "core.h"

namespace craft
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
	
}

#define CRAFT_PROVIDER_DECLARE(x, name, manager) \
	static ::craft::Feature::_ider _id; \
protected: \
	typedef typename ::craft::manager##ProviderManager< x > T_Manager; \
private: \
	friend T_Manager; \
	friend class ::craft::Types; \
public: \
	static constexpr auto craft_c_featureKind = ::craft::FeatureKind::Provider; \
	static constexpr auto craft_c_featureName = name; \
	static inline ::craft::FeatureId craft_s_featureId() { return x::_id.id; } \
public: \
	virtual ::craft::IFeatureManager* craft_featureManager() const override \
	{ return ::craft::types().getManager(x::craft_s_featureId()); } \
private:

#define CRAFT_PROVIDER_DEFINE(x) \
	::craft::Feature::_ider x::_id;
