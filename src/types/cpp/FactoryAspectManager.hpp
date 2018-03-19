#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** FactoryAspectManager
	******************************************************************************/

	template<typename TAspect>
	class FactoryAspectManager
		: public IAspectManager
	{
	protected:
		std::map<cpp::TypePtr, IAspectFactory<TAspect>*> _factories;

		struct _t_instanceCounted
		{
			TAspect* aspect;
			int count;
		};
		std::map<void*, _t_instanceCounted> _instances;
	public:
		inline virtual ~FactoryAspectManager()
		{
			// TODO delete factories
		}

		//
		// IFeatureManager
		//
		virtual cpp::TypePtr featureDesc() const override
		{
			return TAspect::craft_s_typeDesc();
		}
		inline virtual std::string featureName() const override
		{
			return std::string(TAspect::craft_s_featureName());
		}

		virtual void purgeType(cpp::TypePtr tid) noexcept override
		{
			auto it = _factories.find(tid);
			if (it == _factories.end())
				return;
			delete it->second;
		}

		virtual std::vector<cpp::TypePtr> supportedTypes() const override
		{
			return std::vector<cpp::TypePtr>(
				stdext::key_iterator(_factories.begin()),
				stdext::key_iterator(_factories.end()));
		}

		//
		// IAspectManager
		//
		inline virtual bool hasAspect(cpp::TypePtr tid, void* instance) override
		{
			return  _factories.find(tid) != _factories.end();
		}

		inline virtual Aspect* getAspect(cpp::TypePtr tid, void* instance) override
		{
			auto it_f = _factories.find(tid);

			if (it_f == _factories.end())
				throw stdext::exception("FactoryAspectManager.getAspect() for `{0}` Type `{1}` not found", this->featureDesc().toString(), tid.toString());

			auto factory = it_f->second;

			if (!factory->isMultiUseSafe())
			{
				return factory->build(this, instance);
			}

			auto it_i = _instances.find(instance);
			if (it_i == _instances.end())
			{
				TAspect* aspect = factory->build(this, instance);
				_instances[instance] = { aspect, 0 };
				return aspect;
			}
			else
			{
				it_i->second.count++;
				return it_i->second.aspect;
			}
		}
		inline virtual void deleteAspect(Aspect* aspect) override
		{
			assert(aspect != nullptr
				&& "Ensure aspect is not nullptr.");
			assert(aspect->craft_featureManager() == this
				&& "Ensure advertised manager matches.");

			auto inst = aspect->craft_featuredInstance();
			auto it = _instances.find(inst.get());

			if (it == _instances.end())
			{
				_factories[inst.typeId()]->destroy((TAspect*)aspect);
				return;
			}
			else
			{
				if (--(it->second.count) <= 0)
				{
					_factories[inst.typeId()]->destroy((TAspect*)aspect);
				}
				return;
			}
		}

		//
		// FactoryAspectManager
		//
		inline virtual void addFactory(cpp::TypePtr tid, IAspectFactory<TAspect>* factory)
		{
			/*
			assert(factory->craft_featuredTypeId() == tid
			&& "Ensure advertised types match.");
			assert(factory->craft_featureManager() == (IFeatureManager*)this
			&& "Ensure advertised manager matches.");
			*/
			assert(_factories.find(tid) == _factories.end()
				&& "Ensure type is not already registered.");

			_factories[tid] = factory;
		}
	};
}}