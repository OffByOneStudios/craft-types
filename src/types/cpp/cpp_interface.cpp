#include "../common.h"
#include "../core.h"
#include "cpp_interface.h"

#include "Windows.h"

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
	_addEntry({ new std::string("cpp-static-init-finish"), _Entry::Kind::Marker });

	_dllsThatWereStatic = _dllsToUpdate;
	_dllsToUpdate.clear();

	_graph = new Graph();

	// Set up graph and identifiers
	_init_insertEntries(_static_entries, 0);

	// Build up the Runtime and Graph:
	cpp::DefineHelper<void>::_build_default_providers();

	_init_runEntries(_static_entries, 0);
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
void CppSystem::_finish(char const* name)
{
#ifdef _WIN32
	OutputDebugStringA("system._finish()      ");
	OutputDebugStringA(name);
	OutputDebugStringA("\n");
#endif

	auto ret = __dll_region;
	__dll_region = name;

	_addEntry({ new std::string(fmt::format("cpp-library-finish:{0}", ret)), _Entry::Kind::Marker });

	if (_dll_entries.find(ret) != _dll_entries.end())
		_addEntry({ new std::string("cpp-library-already-exists"), _Entry::Kind::Warning });
	_dll_entries[ret] = _current_dll_entries;
	_dllsToUpdate.insert(ret);
	_lastLoadedDll = ret;
	_current_dll_entries = nullptr;
}

void CppSystem::_update()
{
	for (auto d : _dllsToUpdate)
	{
		auto entries = _dll_entries[d];

		_init_insertEntries(entries, 0);
		_init_runEntries(entries, 0);
	}

	_dllsToUpdate.clear();
}

void CppSystem::_addEntry(_Entry && e)
{
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