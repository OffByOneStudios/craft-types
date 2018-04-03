#include "../common.h"
#include "../core.h"
#include "cpp_interface.h"

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
	_addEntry({ new std::string("cpp-static-init-start"), _Entry::Kind::Marker });
}
CppSystem::~CppSystem()
{
}

CppSystem& CppSystem::global_instance()
{
	static CppSystem __global_instance;
	return __global_instance;
}

void CppSystem::_init()
{
	_addEntry({ new std::string("cpp-static-init-finish"), _Entry::Kind::Marker });

	_identifiers = new Identifiers();
	_graph = new Graph();

	_current_dll_entries = new _Entries();

	// Set up graph and identifiers
	for (auto i = 0; i < _static_entries->_entries.size(); ++i)
	{
		auto& entry = _static_entries->_entries[i];
		switch (entry.kind)
		{
			case _Entry::Kind::Type:
			{
				auto td = static_cast<cpp::type_desc*>(entry.ptr);

				cpp::TypeDefineHelper<void> helper(td);
				td->initer(helper);
			} break;

			case _Entry::Kind::Info:
			{
				auto id = static_cast<cpp::info_desc*>(entry.ptr);

				cpp::InfoDefineHelper<void> helper(id);
				id->initer(helper);
			} break;
		}
	}

	// Build up the Runtime and Graph:
	cpp::TypeDefineHelper<void>::_build_default_providers();

	for (auto i = 0; i < _static_entries->_entries.size(); ++i)
	{
		auto& entry = _static_entries->_entries[i];
		switch (entry.kind)
		{
			case _Entry::Kind::Type:
			{
				auto td = static_cast<cpp::type_desc*>(entry.ptr);

				cpp::TypeDefineHelper<void> helper(td);
				td->initer(helper);
			} break;

			case _Entry::Kind::Info:
			{
				auto id = static_cast<cpp::info_desc*>(entry.ptr);

				cpp::InfoDefineHelper<void> helper(id);
				id->initer(helper);
			} break;
		}
	}
}

void CppSystem::_addEntry(_Entry && e)
{
	if (_current_dll_entries == nullptr)
		_static_entries->_entries.push_back(e);
	else
		_current_dll_entries->_entries.push_back(e);
}

void CppSystem::_registerType(cpp::TypePtr tp)
{
	_addEntry({ const_cast<cpp::type_desc*>(tp.desc), _Entry::Kind::Type });
}

void CppSystem::_registerInfo(cpp::info_desc const* info)
{
	_addEntry({ const_cast<cpp::info_desc*>(info), _Entry::Kind::Info });
}