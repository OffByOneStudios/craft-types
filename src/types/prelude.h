#pragma once
#include "types/common.h"

// Forward Declarations
namespace craft {
namespace types
{
	// Defined in `system/graph`
	class TypeStore;
	inline TypeStore& global_store();

	// Defined in `system/graph`
	class TypeStore;
	inline TypeStore& thread_store();

	// Defined in `system/graph`
	struct TypeId;

	// Defined in `cpp/instance_prelude`, `cpp/instance`
	template<typename TType = void, typename T_ = void> struct instance;

	// Defined in `cpp/cpp_interface`
	class CppSystem;
	inline CppSystem& system();

	// Defined in `cpp/cpp_interface`
	template <typename T, typename = void> struct cpptype;

	namespace cpp
	{
		// Defined in `cpp/cpp_interface`?
		struct TypePtr;
	}
}}
