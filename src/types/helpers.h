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

}}