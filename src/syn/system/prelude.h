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

	namespace dll
	{
		inline char const* _begin(char const* name);
		inline void _finish(char const* save, char const* name = nullptr);

		inline void boot();
		inline void update();
		inline void reset();
		inline void load(std::string const& path);
	}
}
