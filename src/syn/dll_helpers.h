#pragma once
#include "syn/common.h"
#include "syn/syn.h"

namespace syn {
namespace dll {

	inline void _boot()
	{
		system();
		system()._init();
	}

	inline char const* _begin(char const* name)
	{
		return CppSystem::_begin(name);
	}

	inline void _finish(char const* save, char const* name = nullptr)
	{
		system()._finish(save, name);
	}

	inline void load(std::string const& path)
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
