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

	inline void dll_begin(char const* name)
	{
		system();
		system()._begin(name);
	}

	inline void dll_end()
	{
		system()._update();
	}
}}