#include "common.h"
#include "core.h"
#include "identifiers.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Identifiers
******************************************************************************/

Identifiers Identifiers::__global_instance(true);
thread_local Identifiers Identifiers::__threadlocal_instance(true);

Identifiers::Identifiers(bool singleton)
	: _contents(nullptr)
{
	if (singleton)
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
	else
	{
		_contents = new Identifiers::_Data();
		_contents->refcount = 1;
	}
}
Identifiers::Identifiers(Identifiers const& that)
{
	_contents = that._contents;
	_contents->refcount += 1;
}
Identifiers::~Identifiers()
{
	_contents->refcount -= 1;
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

	auto id = TypeId(_contents->types.size() + 1);

	_contents->types.push_back({ ptr, ptr_type, id });
	_contents->types_byPtr[ptr] = id;

	return id;
}

Identifiers::Record const& Identifiers::get(TypeId const& id) const
{
	auto v = id.id - 1;
	if (v > _contents->types.size())
		throw type_not_found_by_identifer_error("Identifier {0} out of rance.", id.id);
	return _contents->types[v];
}
TypeId Identifiers::id(void* const& id) const
{
	auto it = _contents->types_byPtr.find(id);
	if (it == _contents->types_byPtr.end())
		throw type_identifer_not_found_error("Could not find an identifer for the given pointer.");
	return it->second;
}

void Identifiers::import(Identifiers const& other)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	for (auto t : other._contents->types)
	{
		add(t.ptr, t.ptr_type);
	}
}

Identifiers::MarkerId Identifiers::getMarker(std::string const& name) const
{
	return _contents->markers_byName[name];
}
Identifiers::MarkerId Identifiers::startMarker(Identifiers::Marker const& marker)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	_contents->markers.push_back({ marker, _contents->types.size(), 0 });

	return _contents->markers.size() - 1;
}
void Identifiers::endMarker(Identifiers::MarkerId markerId)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	_contents->markers[markerId].end = _contents->types.size();
}
size_t Identifiers::countOfMarker(Identifiers::MarkerId markerId) const
{
	_Marker m = _contents->markers[markerId];

	if (m.end == 0)
		return _contents->types.size() - m.start;
	return m.end - m.start;
}