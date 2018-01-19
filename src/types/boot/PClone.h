#pragma once
#include "../common.h"

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
		CRAFT_TYPES_EXPORTED CRAFT_PROVIDER_DECLARE(PClone, "types.clone", SingletonProviderManager);

	public:
		virtual instance<> clone(instance<>) const noexcept = 0;
	};

	/******************************************************************************
	** DefaultCopyConstructor
	******************************************************************************/

	template <typename T>
	class DefaultCopyConstructor
		: public Implements<PClone>::For<T>
	{
	public:
		inline virtual instance<> clone(instance<> in) const noexcept override
		{
			instance<T> ret = instance<T>::make(*in.asType<T>());

			return (instance<>)ret;
		}
	};

	/******************************************************************************
	** FunctionalCopyConstructor
	******************************************************************************/

	template <typename T>
	class FunctionalCopyConstructor
		: public Implements<PClone>::For<T>
	{
		std::function<instance<T>(instance<T>)> _clone;
	public:

		inline FunctionalCopyConstructor(std::function<instance<T>(instance<T>)> clone)
			: _clone(clone)
		{

		}

		inline virtual instance<> clone(instance<> in) const noexcept override
		{
			try
			{
				instance<T> ret = _clone(in.asType<T>());

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
