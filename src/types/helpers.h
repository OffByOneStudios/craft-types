#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types {

	inline void boot()
	{
		system();
		system()._init();
	}

	inline char const* dll_begin(char const* name)
	{
		return CppSystem::_begin(name);
	}

	inline void dll_finish(char const* name)
	{
		system()._finish(name);
	}
}}
