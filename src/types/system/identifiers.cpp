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

TypeId Identifiers::add(Graph::Node* node_ptr)
{
	std::lock_guard<std::recursive_mutex> lock(_contents->operation);

	auto it = _contents->types.insert({ node_ptr, node_ptr->value });
	auto id = _contents->types.get_index_from_iterator(it) + 1;
	_contents->types_byPtr[node_ptr] = id;
	_contents->types_byPtr[node_ptr->value] = id;

	return id;
}

Identifiers::Record const& Identifiers::get(TypeId id) const
{
	auto it = _contents->types_byPtr.find((void*)id.node);
	if (it == _contents->types_byPtr.end())
		throw type_identifer_not_found_error("Could not find an identifer for the given node.");
	return *_contents->types.get_iterator_from_index(it->second - 1);
}
Identifiers::Record const& Identifiers::get(void* ptr) const
{
	auto it = _contents->types_byPtr.find((void*)ptr);
	if (it == _contents->types_byPtr.end())
		throw type_identifer_not_found_error("Could not find an identifer for the given static_desc.");
	return *_contents->types.get_iterator_from_index(it->second - 1);
}
