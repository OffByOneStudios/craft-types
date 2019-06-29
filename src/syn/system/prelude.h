#pragma once
#include "syn/common.h"

// Forward Declarations
namespace syn
{
	namespace details
	{
		struct TypeIdForwardDeclare;
	}

	// Defined in `system/graph`
	struct TypeId;

	// Defined in `system/store`
	class TypeStore;

	inline TypeStore& static_store();
	inline TypeStore& thread_store();
}
