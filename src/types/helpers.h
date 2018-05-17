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

	inline void _dll_finish(char const* save, char const* name = nullptr)
	{
		system()._finish(save, name);
	}

	inline void load_dll(std::string const& path)
	{
		auto target = path::normalize(path);
#ifdef _WIN32
		auto handle = LoadLibraryA(target.c_str());
		if (handle == nullptr) throw stdext::exception(util::platform::windows::GetLastErrorAsString());
#else
		if (!dlopen(path.c_str(), RTLD_NOW)) throw stdext::exception(dlerror());
#endif
		system()._update();
	}
}}
