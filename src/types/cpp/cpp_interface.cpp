#include "../common.h"
#include "../core.h"
#include "../graph/CppType.h"
#include "cpp_interface.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Object Virtual Base
******************************************************************************/

Object::Object()
{
	header = nullptr;
}

void Object::craft_setupInstance()
{
	header = new InstanceHeader(craft_typeDesc(), craft_instancePointer());
}

Object::~Object()
{
}

/******************************************************************************
** CppSystem
******************************************************************************/

CppSystem CppSystem::__global_instance;

CppSystem::CppSystem()
	: _identifiers(_identifiers)
	, _type_cpp(_type_cpp)
	, operation()
{
	__s_init();
}
CppSystem::~CppSystem()
{

}

void CppSystem::__s_init()
{
	static std::once_flag flag;
	std::call_once(flag, []() {
		global_instance()._identifiers = new Identifiers();
		global_instance()._type_cpp = global_instance()._identifiers->add(new RootType("cpp"), 0);
		global_instance()._identifiers->startMarker({ &global_instance(), 0, "cpp:static-init" });
	});
}

CppSystem& CppSystem::global_instance()
{
	return __global_instance;
}

Identifiers const& CppSystem::identifiers() const
{
	return *_identifiers;
}

void CppSystem::init()
{
	cpp::TypeDefineHelper<void>::_build_default_providers();

	types::identifiers().import(*_identifiers);

	for (auto i = 1; i < _identifiers->count(); ++i)
	{
		auto record = _identifiers->get(i);
		if (record.ptr_type == _type_cpp)
		{
			auto td = static_cast<cpp::type_desc*>(record.ptr);

			cpp::TypeDefineHelper<void> helper(td);

			td->initer(helper);
		}
	}
}

void CppSystem::_registerType(cpp::TypePtr tp)
{
	__s_init();

	_identifiers->add(const_cast<cpp::type_desc*>(tp.desc), _type_cpp);
}
