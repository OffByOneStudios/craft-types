#pragma once
#include "../common.h"
#include "../core.h"

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
	protected:
		std::map<cpp::TypePtr, TProvider*> _singletons;
	public:
		virtual ~SingletonProviderManager() = default;

		//
		// IFeatureManager
		//
		virtual cpp::TypePtr featureDesc() const override
		{
			return TProvider::craft_s_typeDesc();
		}
		virtual std::string featureName() const override
		{
			return std::string(TProvider::craft_s_featureName());
		}

		virtual void purgeType(cpp::TypePtr tid) noexcept override
		{
			auto it = _singletons.find(tid);
			if (it == _singletons.end())
				return;
			delete it->second;
		}

		virtual std::vector<cpp::TypePtr> supportedTypes() const override
		{
			return std::vector<cpp::TypePtr>(
				stdext::key_iterator(_singletons.begin()),
				stdext::key_iterator(_singletons.end()));
		}

		//
		// IProviderManager
		//
		inline virtual bool hasProvider(cpp::TypePtr tid) override
		{
			return _singletons.find(tid) != _singletons.end();
		}
		inline virtual Provider* getProvider(cpp::TypePtr tid) override
		{
			auto it = _singletons.find(tid);
			if (it == _singletons.end())
				throw stdext::exception("SingletonProviderManager.getProvider() for `{0}` Type `{1}` not found", this->featureDesc().toString(), tid.toString());
			return it->second;
		}
		inline virtual void releaseProvider(Provider* provider) override
		{
			return;
		}

		//
		// SingletonProviderManager
		//
		inline virtual void addSingleton(cpp::TypePtr tid, TProvider* singleton)
		{
			//assert(singleton->craft_featuredTypeId() == TypeId(tid)
			//	&& "Ensure advertised types match.");
			assert(singleton->craft_featureManager() == (IFeatureManager*)this
				&& "Ensure advertised manager matches.");
			assert(_singletons.find(tid) == _singletons.end()
				&& "Ensure singleton is not already registered.");

			_singletons[tid] = singleton;
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
		std::map<std::string, cpp::TypePtr> _index;
	public:
		virtual ~NamedSingletonProviderManager() = default;

		inline virtual void addSingleton(cpp::TypePtr tid, TProvider* singleton) override
		{
			SingletonProviderManager<TProvider>::addSingleton(tid, singleton);
			_index[singleton->provider_index()] = tid;
		}

		inline virtual cpp::TypePtr index(std::string const& s) const override
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
		std::map<std::string, std::set<cpp::TypePtr>> _tags;
	public:
		virtual ~TaggedSingletonProviderManager() = default;

		inline virtual void addSingleton(cpp::TypePtr tid, TProvider* singleton) override
		{
			SingletonProviderManager<TProvider>::addSingleton(tid, singleton);

			for (auto tag : singleton->provider_tags())
			{
				_tags[tag].insert(tid);
			}
		}

		inline virtual std::set<cpp::TypePtr> filter(std::string const& s) const override
		{
			auto it = _tags.find(s);
			if (it == _tags.end())
				return { };
			return it->second;
		}
	};

	/******************************************************************************
	** InitializedSingletonProviderManager
	******************************************************************************/

	template<typename TProvider>
	class InitializedSingletonProviderManager
		: public SingletonProviderManager<TProvider>
	{
		virtual ~InitializedSingletonProviderManager() = default;

		bool _is_inited;
	public:
		inline void init()
		{
			if (!_is_inited)
			{
				_is_inited = true;
				for (auto f : this->_singletons)
				{
					 auto pro = f.second;
					 pro->init();
				}
			}
		}

		inline virtual void addSingleton(TypeId tid, TProvider* singleton) override
		{
			SingletonProviderManager<TProvider>::addSingleton(tid, singleton);

			if (_is_inited)
			{
				singleton->init();
			}
		}
	};
}}
