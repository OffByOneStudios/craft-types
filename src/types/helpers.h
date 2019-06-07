#pragma once
#include "types/common.h"
#include "core.h"

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
		auto target = std::filesystem::path(path).lexically_normal();
#ifdef _WIN32
		auto handle = LoadLibraryW(target.c_str());
		if (handle == nullptr) throw stdext::exception(stdext::platform::windows::GetLastErrorAsString());
#else
		if (!dlopen(path.c_str(), RTLD_NOW)) throw stdext::exception(dlerror());
#endif
		system()._update();
	}
}}
