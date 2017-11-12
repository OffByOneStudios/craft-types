#pragma once
#include "common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** PConversion
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PConversion abstract
		: public Provider
	{
		CRAFT_TYPES_EXPORTED CRAFT_PROVIDER_DECLARE(PConversion, "types.convert", SingletonProviderManager);

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
}}
