#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** SingletonProviderManager
	******************************************************************************/

	template<typename TProvider>
	class SingletonProviderManager
		: public IProviderManager
	{
		std::map<TypeId, TProvider*> _singletons;
	public:
		virtual ~SingletonProviderManager() = default;

		//
		// IFeatureManager
		//
		virtual FeatureId featureId() const override
		{
			return TProvider::craft_s_featureId();
		}
		virtual std::string featureName() const override
		{
			return std::string(TProvider::craft_c_featureName);
		}

		virtual void purgeType(TypeId tid) noexcept override
		{
			auto it = _singletons.find(tid);
			if (it == _singletons.end())
				return;
			delete it->second;
		}

		virtual std::vector<TypeId> supportedTypes() const override
		{
			return std::vector<TypeId>(
				stdext::key_iterator(_singletons.begin()),
				stdext::key_iterator(_singletons.end()));
		}

		//
		// IProviderManager
		//
		inline virtual bool hasProvider(TypeId tid) override
		{
			return _singletons.find(tid) != _singletons.end();
		}
		inline virtual Provider* getProvider(TypeId tid) override
		{
			auto it = _singletons.find(tid);
			if (it == _singletons.end())
				throw stdext::exception("SingletonProviderManager.getProvider() for `{0}` Type `{1}` not found", this->featureId().toString(), tid.toString());
			return it->second;
		}
		inline virtual void releaseProvider(Provider* provider) override
		{
			return;
		}

		//
		// SingletonProviderManager
		//
		inline virtual void addSingleton(TypeId tid, TProvider* singleton)
		{
			assert(singleton->craft_featuredTypeId() == tid
				&& "Ensure advertised types match.");
			assert(singleton->craft_featureManager() == (IFeatureManager*)this
				&& "Ensure advertised manager matches.");
			assert(_singletons.find(tid) == _singletons.end()
				&& "Ensure singleton is not already registered.");

			_singletons[tid] = singleton;
		}

		//
		// Template IFeatureManager
		//
		inline virtual void add(TypeId tid, FeatureId fid, TProvider* object)
		{
			assert(TProvider::craft_s_featureId() == fid
				&& "Ensure advertised features match.");
			addSingleton(tid, object);
		}
	};

	/******************************************************************************
	** NamedSingletonProviderManager
	******************************************************************************/

	template<typename TProvider>
	class NamedSingletonProviderManager
		: public SingletonProviderManager<TProvider>
		, public IIndexedProviderManager
	{
		std::map<std::string, TypeId> _index;
	public:
		virtual ~NamedSingletonProviderManager() = default;

		inline virtual void addSingleton(TypeId tid, TProvider* singleton) override
		{
    	SingletonProviderManager<TProvider>::addSingleton(tid, singleton);
			_index[singleton->provider_index()] = tid;
		}

		inline virtual TypeId index(std::string const& s) const override
		{
			auto it = _index.find(s);
			if (it == _index.end())
				return 0;
			return it->second;
		}
	};
	/******************************************************************************
	** TaggedSingletonProviderManager
	******************************************************************************/

	template<typename TProvider>
	class TaggedSingletonProviderManager
		: public SingletonProviderManager<TProvider>
		, public ITaggedProviderManager
	{
		std::map<std::string, std::set<TypeId>> _tags;
	public:
		virtual ~TaggedSingletonProviderManager() = default;

		inline virtual void addSingleton(TypeId tid, TProvider* singleton) override
		{
			SingletonProviderManager<TProvider>::addSingleton(tid, singleton);

			for (auto tag : singleton->provider_tags())
			{
				_tags[tag].insert(tid);
			}
		}

		inline virtual std::set<TypeId> filter(std::string const& s) const override
		{
			auto it = _tags.find(s);
			if (it == _tags.end())
				return { };
			return it->second;
		}
	};
}}
