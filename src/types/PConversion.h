#pragma once
#include "common.h"

namespace craft {
	/******************************************************************************
	** PConversion
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PConversion abstract
		: public Provider
	{
		CRAFT_TYPE_EXPORTED CRAFT_PROVIDER_DECLARE(PConversion, "types.convert", Singleton);

	public:
		virtual std::set<TypeId> const& possibleConversions(instance<> converting, TypeId toType) const = 0;
		virtual instance<> convertTo(instance<> converting, TypeId toType) const = 0;
	};

	/******************************************************************************
	** ObjectConversion
	******************************************************************************/

	template <typename TType>
	class ObjectConversion
		: public Implements<PConversion>::For<TType>
	{
	private:
		
	};
}
