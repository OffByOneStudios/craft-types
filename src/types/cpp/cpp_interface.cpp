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
	craft_header = new InstanceHeader(craft_typeDesc(), craft_instancePointer());
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
					case CppStaticDescKindEnum::None:
						break;
					default:
					case CppStaticDescKindEnum::Object:
					{
						auto node = _graph->addNode(_graph->meta<GraphCppStatic>(), sd);

						_identifiers->add(node);
						
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

	_identifiers = new Identifiers();
	_graph = new Graph();

	_current_dll_entries = new _Entries();

	// Set up graph and identifiers
	_init_insertEntries(_static_entries, 0);

	// Build up the Runtime and Graph:
	cpp::DefineHelper<void>::_build_default_providers();

	_init_runEntries(_static_entries, 0);
}

char const* CppSystem::_begin(char const* name)
{
	OutputDebugStringA("system::_begin()      ");
	OutputDebugStringA(name);
	OutputDebugStringA("\n");
	// WARNING !! WARNING !! WARNING
	// This function is called pre-C++-runtime initalize
	// * Static initalizers have not ran
	// * Exceptions are not configured
	// * Stack safety has not been established
	// Be very careful
	// WARNING !! WARNING !! WARNING

	auto ret = __dll_region;
	__dll_region = name;
	return ret;
}
void CppSystem::_finish(char const* name)
{
	OutputDebugStringA("system._finish()      ");
	OutputDebugStringA(name);
	OutputDebugStringA("\n");

	auto ret = __dll_region;
	__dll_region = name;

	_addEntry({ new std::string(fmt::format("cpp-library-finish:{0}", ret)), _Entry::Kind::Marker });
	// TODO insert the begin marker
}

void CppSystem::_update()
{
	auto start = 0;
	for (start = 0; start < _current_dll_entries->_entries.size(); ++start)
	{
		auto& entry = _current_dll_entries->_entries[start];
		if (entry.kind == _Entry::Kind::Marker)
		{
			break;
		}
	}
	if (start == _current_dll_entries->_entries.size()) start = 0;
	else if (start == _current_dll_entries->_entries.size() - 1) return; //Multiple invocations to rebuild without new types

	_init_insertEntries(_current_dll_entries, start);
	_init_runEntries(_current_dll_entries, start);
}

void CppSystem::_addEntry(_Entry && e)
{
	if (_current_dll_entries == nullptr)
		_static_entries->_entries.push_back(e);
	else
		_current_dll_entries->_entries.push_back(e);
}

void CppSystem::_register(cpp::static_desc const* info)
{
	_addEntry({ const_cast<cpp::static_desc*>(info), _Entry::Kind::StaticDesc });
}

