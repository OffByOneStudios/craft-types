#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** InstanceAspectManager
	******************************************************************************/

	template<typename TAspect>
	class InstanceAspectManager
		: public FactoryAspectManager<TAspect>
	{
	protected:
		std::map<void*, TAspect*> _overrideInstances;

	public:
		virtual ~InstanceAspectManager() = default;

		//
		// IAspectManager
		//
		inline virtual bool hasAspect(cpp::TypePtr tid, void* instance)
		{
			return _overrideInstances.find(instance) != _overrideInstances.end() || FactoryAspectManager<TAspect>::hasAspect(tid, instance);
		}
		inline virtual Aspect* getAspect(cpp::TypePtr tid, void* instance)
		{
			auto it = _overrideInstances.find(instance);
			if (it == _overrideInstances.end())
			{
				return FactoryAspectManager<TAspect>::getAspect(tid, instance);
			}
			else
			{
				return it->second;
			}
		}
		inline virtual void deleteAspect(Aspect* aspect)
		{
			auto it = _overrideInstances.find(aspect->craft_featuredInstance().get());

			if (it == _overrideInstances.end())
			{
				return FactoryAspectManager<TAspect>::deleteAspect(aspect);
			}
			else if ((Aspect*)it->second != aspect)
			{
				throw stdext::exception("FactoryAspectManager::deleteAspect() | Asked to delete aspect for an instance it does not advertise.");
			}
			else
			{
				return;
			}
		}

		//
		// InstanceAspectManager
		//
		inline virtual void addInstanceAspect(void* instance, TAspect* aspect)
		{
			//assert(aspect->craft_featuredTypeId() == tid
			//	&& "Ensure advertised types match.");
			assert(aspect->craft_featureManager() == (IFeatureManager*)this
				&& "Ensure advertised manager matches.");
			assert(_overrideInstances.find(instance) == _overrideInstances.end()
				&& "Ensure singleton is not already registered.");

			_overrideInstances[instance] = aspect;
		}
	};
}}