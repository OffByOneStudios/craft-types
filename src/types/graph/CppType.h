#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types {

	namespace cpp
	{
		struct type_desc;
	}

	class CppType final
	{
		cpp::TypePtr _desc;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED CppType(cpp::TypePtr desc);
		CRAFT_TYPES_EXPORTED ~CppType();
	};
}}
