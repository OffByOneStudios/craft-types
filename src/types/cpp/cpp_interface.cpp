#include "../common.h"
#include "../core.h"
#include "cpp_interface.h"

#ifdef _WIN32
#include "Windows.h"
#endif
// here be dragons

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Object Virtual Base
******************************************************************************/

Object::Object()
{
	craft_header = nullptr;
}

void Object::craft_setupInstance()
{

}

Object::~Object()
{
}

/******************************************************************************
** CppSystem
******************************************************************************/

CppSystem::CppSystem()
{
	_static_entries = new _Entries();
	_addEntry({ new std::string("cpp-static-init-begin"), _Entry::Kind::Marker });
}
CppSystem::~CppSystem()
{
}

char const* CppSystem::__dll_region = nullptr;

CppSystem& CppSystem::global_instance()
{
	static CppSystem __global_instance;
	return __global_instance;
}

void CppSystem::_init_insertEntries(_Entries* entries, size_t start)
{
	//std::cerr << "CppSystem::_init_insertEntries:" << entries->_entries.size() << std::endl;
	for (auto i = start; i < entries->_entries.size(); ++i)
	{
		auto& entry = entries->_entries[i];
		switch (entry.kind)
		{
			case _Entry::Kind::StaticDesc:
			{
				cpp::static_desc* sd = (cpp::static_desc*)entry.ptr;

				switch (sd->kind)
				{
					default:
					case CppStaticDescKindEnum::None:
						break;
					case CppStaticDescKindEnum::Object:
					{
						sd->node = _graph->addNode(_graph->meta<GraphNodeCppObject>(), sd);
					} break;

					case CppStaticDescKindEnum::RawType:
					{
						sd->node = _graph->addNode(_graph->meta<GraphNodeCppClass>(), sd);
					} break;
					case CppStaticDescKindEnum::LegacyProvider:
					case CppStaticDescKindEnum::LegacyAspect:
					{
						sd->node = _graph->addNode(_graph->meta<GraphNodeCppFeatureLegacy>(), sd);
					} break;
					case CppStaticDescKindEnum::MultiMethod:
					{
						sd->node = _graph->addNode(_graph->meta<GraphNodeCppMultiMethod>(), sd);
					} break;
					case CppStaticDescKindEnum::UserInfo:
					{
						sd->node = _graph->addNode(_graph->meta<GraphNodeCppUserInfo>(), sd);
					} break;
				}
			} break;
		}
	}
}

void CppSystem::_init_runEntries(_Entries* entries, size_t start)
{
	//std::cerr << "CppSystem::_init_runEntries:" << entries->_entries.size() << std::endl;
	for (auto i = start; i < entries->_entries.size(); ++i)
	{
		auto& entry = entries->_entries[i];
		switch (entry.kind)
		{
			case _Entry::Kind::StaticDesc:
			{
				auto td = static_cast<cpp::static_desc*>(entry.ptr);

				if (td->initer == nullptr) continue;

				cpp::DefineHelper<void> helper(td);
				td->initer(helper);
			} break;
		}
	}
}

void CppSystem::_init()
{
	//std::cerr << "CppSystem::_init:" << _static_entries->_entries.size() << std::endl;
	//std::cerr << "CppSystem::_init:toup:" << (_dllsToUpdate.size() == 0 ? 0 : _dll_entries[*_dllsToUpdate.begin()]->_entries.size()) << std::endl;
	//std::cerr << "CppSystem::_init:stat:" << (_dllsThatWereStatic.size() == 0 ? 0 : _dll_entries[*_dllsThatWereStatic.begin()]->_entries.size()) << std::endl;

	_addEntry({ new std::string("cpp-static-init-finish"), _Entry::Kind::Marker });

	_dllsThatWereStatic = _dllsToUpdate;
	_dllsToUpdate.clear();
	_lastLoadedDll = "";
	_current_dll_entries = nullptr;

	_graph = new Graph();

	// Set up graph and identifiers
	_init_insertEntries(_static_entries, 0);

	// Build up the Runtime and Graph:
	cpp::DefineHelper<void>::_build_default_providers();

	_init_runEntries(_static_entries, 0);

	//std::cerr << "CppSystem::_init:curr" << (_current_dll_entries == nullptr ? "OKOKOK" : "BADBAD") << std::endl;
	//std::cerr << "CppSystem::_init:toup" << (_dllsToUpdate.size() == 0 ? 0 : _dll_entries[*_dllsToUpdate.begin()]->_entries.size()) << std::endl;
}

bool CppSystem::_hasInited()
{
	return _graph != nullptr;
}

char const* CppSystem::_begin(char const* name)
{
	// WARNING !! WARNING !! WARNING
	// This function is called pre-C++-runtime initalize
	// * Static initalizers have not ran
	// * Exceptions are not configured
	// * C++ memory management features are not configured
	// Be very careful
	// WARNING !! WARNING !! WARNING

#ifdef _WIN32
	OutputDebugStringA("system::_begin()      ");
	OutputDebugStringA(name);
	OutputDebugStringA("\n");
#endif

	auto ret = __dll_region;
	__dll_region = name;
	return ret;
}
void CppSystem::_finish(char const* save, char const* name)
{
#ifdef _WIN32
	OutputDebugStringA("system._finish()      ");
	OutputDebugStringA(name);
	OutputDebugStringA("\n");
#endif
	if (name == nullptr)
	{
		name = __dll_region;
		__dll_region = save;
	}

	_addEntry({ new std::string(fmt::format("cpp-library-finish:{0}", name)), _Entry::Kind::Marker });

	if (_dll_entries.find(name) != _dll_entries.end())
		_addEntry({ new std::string("cpp-library-already-exists"), _Entry::Kind::Warning });
	_dll_entries[name] = _current_dll_entries;
	_dllsToUpdate.insert(name);
	_lastLoadedDll = name;
	_current_dll_entries = nullptr;
}

void CppSystem::_update()
{
	//std::cerr << "CppSystem::_update:" << _dll_entries[*_dllsToUpdate.begin()]->_entries.size() << std::endl;
	for (auto d : _dllsToUpdate)
	{
		_init_insertEntries(_dll_entries[d], 0);
	}
	for (auto d : _dllsToUpdate)
	{
		_init_runEntries(_dll_entries[d], 0);
	}

	_dllsToUpdate.clear();
}

void CppSystem::_addEntry(_Entry && e)
{
	if (e.kind == _Entry::Kind::Marker) {}
		//std::cerr << "CppSystem::_addEntry:" << _hasInited() << ":Marker:" << *(std::string*)e.ptr << std::endl;
	else if (e.kind == _Entry::Kind::StaticDesc)
	{
		auto sd = ((cpp::static_desc*)e.ptr);

		//std::cerr << "CppSystem::_addEntry:" << _hasInited() << ":Desc:" << (uint32_t)sd->kind << std::endl;
	}
	else if (e.kind == _Entry::Kind::Warning)
		//std::cerr << "CppSystem::_addEntry:" << _hasInited() << ":Warning:" << *(std::string*)e.ptr << std::endl;

	if (!_hasInited())
		_static_entries->_entries.push_back(e);

	if (_current_dll_entries == nullptr)
		_current_dll_entries = new _Entries();

	_current_dll_entries->_entries.push_back(e);
}

void CppSystem::_register(cpp::static_desc const* info)
{
	_addEntry({ const_cast<cpp::static_desc*>(info), _Entry::Kind::StaticDesc });
}

std::string CppSystem::getLastLibraryName()
{
	return _lastLoadedDll;
}
size_t CppSystem::getLibraryCount(std::string const& dll)
{
	return _dll_entries[dll]->_entries.size();
}
cpp::TypePtr CppSystem::getLibraryEntry(std::string const& dll, size_t index)
{
	return reinterpret_cast<cpp::static_desc*>(_dll_entries[dll]->_entries[index].ptr);
}