#pragma once
#include "common.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** PStringer
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PStringer abstract
		: public Provider
	{
		CRAFT_TYPE_EXPORTED CRAFT_PROVIDER_DECLARE(PStringer, "types.str", SingletonProviderManager);

	public:
		virtual std::string toString(instance<> inst) const = 0;
	};

	/******************************************************************************
	** FunctionalStringer
	******************************************************************************/

	namespace _details
	{
		static std::function<std::string(instance<>)> FunctionalStringer_defaultReport;
	}

	template <typename T>
	class FunctionalStringer
		: public Implements<PStringer>::For<T>
	{
		std::function<std::string(instance<>)> _stringer;

	public:
		inline FunctionalStringer(std::function<std::string(instance<T>)> const& stringer)
			: _stringer([stringer](instance<> inst) { return stringer(inst.asType<T>()); })
		{ }

		inline virtual std::string toString(instance<> inst) const override { return _stringer(inst); }
	};
}}