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
		using FinalInstance = typename TPolicy<TType>::FinalInstance;

		using FinalInstance::FinalInstance;
	};
}
