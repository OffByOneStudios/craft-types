#pragma once
#include "syn/syn.h"

namespace syn
{
	template<typename TType, template <typename> typename TPolicy>
	struct instance final
		: public TPolicy<TType>::FinalInstance
	{
		using FinalInstance = typename TPolicy<TType>::FinalInstance;

		using FinalInstance::FinalInstance;
	};
}
