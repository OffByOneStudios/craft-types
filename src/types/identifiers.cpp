#include "common.h"
#include "identifiers.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Identifiers
******************************************************************************/

Identifiers Identifiers::__global_instance;
thread_local Identifiers Identifiers::__threadlocal_instance;

Identifiers::Identifiers()
	: _contents(nullptr)
{
	if (__global_instance._contents != nullptr)
	{
		__global_instance._contents->refcount += 1;
		_contents = __global_instance._contents;
	}
	else
	{
		_contents = new Identifiers::_Data();
		_contents->refcount = 1;
	}
}
Identifiers::Identifiers(Identifiers const& that)
{

}
Identifiers::~Identifiers()
{

}

Identifiers& Identifiers::global_instance()
{
	return __global_instance;
}
Identifiers& Identifiers::threadlocal_instance()
{
	return __threadlocal_instance;
}

size_t Identifiers::count() const
{
	return _contents->types.size();
}

TypeId Identifiers::add(void* const& ptr, TypeId const& ptr_type)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	auto id = TypeId(_contents->types.size());

	_contents->types.push_back({ ptr, ptr_type, id });
	_contents->types_byPtr[ptr] = id;

	return id;
}
void Identifiers::patch(TypeId const& to_patch, TypeId const& ptr_type)
{

}
Identifiers::Record const& Identifiers::get(TypeId const& id) const
{
	return _contents->types[id.id - 1];
}
TypeId Identifiers::id(void* const& id) const
{
	return _contents->types_byPtr[id];
}

void Identifiers::import(Identifiers const& other)
{
	for (auto t : other._contents->types)
	{
		add(t.ptr, t.ptr_type);
	}
}
