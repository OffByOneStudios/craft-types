#pragma once
#include "../common.h"

namespace craft {
	namespace types
	{

		/******************************************************************************
		** PStringer
		******************************************************************************/

		/* T:
		Provides an interface for naming things, and reversing those names into actual objects.
		*/

		class PRepr abstract
			: public Provider
		{
			CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(craft::types::PRepr, "types.repr", SingletonProviderManager);

		public:
			virtual std::string toRepr(instance<> inst) const = 0;
		};

		/******************************************************************************
		** FunctionalStringer
		******************************************************************************/

		namespace _details
		{
			static std::function<std::string(instance<>)> FunctionalRepr_defaultReport;
		}

		template <typename T>
		class FunctionalRepr
			: public Implements<PRepr>::For<T>
		{
			std::function<std::string(instance<>)> _repr;

		public:
			inline FunctionalRepr(std::function<std::string(instance<T>)> const& repr)
				: _repr([repr](instance<> inst) { return repr(inst.asType<T>()); })
			{ }

			inline virtual std::string toRepr(instance<> inst) const override { return _repr(inst); }
		};
	}
}
