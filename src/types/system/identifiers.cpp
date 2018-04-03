#include "../common.h"
#include "../core.h"
#include "identifiers.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Identifiers
******************************************************************************/

Identifiers::Identifiers()
	: _contents()
{
	_contents = new Identifiers::_Data();
	_contents->refcount = 1;
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

size_t Identifiers::count() const
{
	return _contents->types.size();
}

TypeId Identifiers::add(void* const& ptr, void* const& node_ptr)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	auto it = _contents->types.insert({ ptr, 0, node_ptr });
	auto id = _contents->types.get_index_from_iterator(it);
	_contents->types_byPtr[ptr] = id;
	_contents->types_byPtr[node_ptr] = id;
	it->id = id;

	return id;
}

Identifiers::Record const& Identifiers::get(TypeId const& id) const
{
	auto v = id.id - 1;
	if (v > _contents->types.size())
		throw type_not_found_by_identifer_error("Identifier {0} out of rance.", id.id);
	return *_contents->types.get_iterator_from_index(v);
}
TypeId Identifiers::id(void* const& id) const
{
	auto it = _contents->types_byPtr.find(id);
	if (it == _contents->types_byPtr.end())
		throw type_identifer_not_found_error("Could not find an identifer for the given pointer.");
	return it->second;
}
