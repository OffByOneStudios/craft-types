#pragma once
#include "syn/syn.h"

namespace syn
{
	template<
		typename TType /* = void */,
		template <typename> typename TPolicy /* = instance_policy::CppReferneceCountedActual */
	>
	struct instance final
		: public TPolicy<TType>::FinalInstance
	{
		using ThisType = instance<TType, TPolicy>;
		using FinalInstance = typename TPolicy<TType>::FinalInstance;

		using FinalInstance::FinalInstance;

	public:
		instance(ThisType const& src)
			: FinalInstance()
		{
			instance_policy::copy<ThisType, ThisType>::_(*this, src);
		}

		instance(ThisType&& src)
			: FinalInstance()
		{
			instance_policy::move<ThisType, ThisType>::_(*this, std::move(src));
		}

		ThisType& operator=(ThisType const& src)
		{
			instance_policy::copy<ThisType, ThisType>::_(*this, src);
			return *this;
		}

		ThisType& operator=(ThisType&& src)
		{
			instance_policy::move<ThisType, ThisType>::_(*this, std::move(src));
			return *this;
		}

	public:
		template<typename TSrc>
		instance(TSrc const& src)
			: FinalInstance()
		{
			instance_policy::copy<ThisType, TSrc>::_(*this, src);
		}

		template<typename TSrc>
		instance(TSrc&& src)
			: FinalInstance()
		{
			instance_policy::move<ThisType, TSrc>::_(*this, std::move(src));
		}

		template<typename TSrc>
		ThisType& operator=(TSrc const& src)
		{
			instance_policy::copy<ThisType, TSrc>::_(*this, src);
			return *this;
		}

		template<typename TSrc>
		ThisType& operator=(TSrc&& src)
		{
			instance_policy::move<ThisType, TSrc>::_(*this, std::move(src));
			return *this;
		}
	};
}
