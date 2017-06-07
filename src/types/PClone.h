#pragma once
#include "common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** PClone
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PClone abstract
		: public Provider
	{
		CRAFT_TYPE_EXPORTED CRAFT_PROVIDER_DECLARE(PClone, "types.clone", SingletonProviderManager);

	public:
		virtual instance<> clone(instance<>) = 0;
	};

	/******************************************************************************
	** ObjectConversion
	******************************************************************************/

	template <typename T>
	class DefaultCopyConstructor
		: public Implements<PClone>::For<T>
	{
	public:
		inline virtual instance<> parse(instance<> in) const noexcept override
		{
			try
			{
				instance<T> ret = instance<T>::make(*in.asType<T>());

				return (instance<>)ret;
			}
			catch (...)
			{
				// TODO, create and return an error type wrapping the exception
				return instance<>();
			}
		}
	};
}}
