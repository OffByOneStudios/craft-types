
#pragma once
#include "syn/syn.h"

namespace syn
{
	CULTLANG_SYNDICATE_EXPORTED bool is_a(TypeId most_specific, TypeId less_specific);

	CULTLANG_SYNDICATE_EXPORTED TypeId basic_dispatch(TypeId dispatcher, TypeId* type_args, size_t count, void* value_args = nullptr, TypeId previous_call = nullptr);
}
