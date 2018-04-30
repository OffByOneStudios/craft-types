#pragma once
#include "common.h"
#include "core.h"

#include "util/platform_windows.h"

namespace craft {
namespace types {

	inline void boot()
	{
		system();
		system()._init();
	}

	inline char const* _dll_begin(char const* name)
	{
		return CppSystem::_begin(name);
	}

	inline void _dll_finish(char const* name)
	{
		system()._finish(name);
	}

	inline void load_dll(std::string const& path)
	{
		auto target = path::normalize(path);
#ifdef _WIN32
		auto handle = LoadLibraryA(target.c_str());
		if (handle == nullptr) throw stdext::exception(util::platform::windows::GetLastErrorAsString());
#else
		throw stdext::exception("load_dll not implemented on platform");
#endif
		system()._update();
	}
}}
