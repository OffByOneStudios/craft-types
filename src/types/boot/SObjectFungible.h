#pragma once
#include "../common.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** SObjectFungible
	******************************************************************************/

	class SObjectFungible abstract
		: public Aspect
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(SObjectFungible, "types.fungible", FactoryAspectManager);

	public:
		/* T:
		Provides the objectFungible's update when it is wholly mutated or replaced.

		* New object : when newInst != nullptr.
		* Mutated    : when newInst == nullptr.

		May be null to not support fungibility on a per instance basis.
		*/
		virtual Signal<void(instance<> origInst, instance<> newInst)>& objectFungible_onUpdate() = 0;

	//
	// Helpers
	//
	public:
		inline virtual void objectFungible_emitUpdated(instance<> newInst = nullptr)
		{
			objectFungible_onUpdate().emit(craft_featuredInstance(), newInst);
		}
	};

	/******************************************************************************
	** BasicObjectFungible
	******************************************************************************/

	class BasicObjectFungible abstract
		: public Implements<SObjectFungible>
	{
	private:
		Signal<void(instance<> origInst, instance<> newInst)> _fungible_signal;

	public:
		CRAFT_TYPES_EXPORTED virtual Signal<void(instance<> origInst, instance<> newInst)>& objectFungible_onUpdate() override;
	};
}}
