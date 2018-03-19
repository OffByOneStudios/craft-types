#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types {

	inline void boot_core()
	{
		identifiers();
		graph();
	}

	inline void boot_with_cpp()
	{
		boot_core();

		system();
		system().init();
	}

}}