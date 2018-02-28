#include "../common.h"
#include "graph.h"
#include "TypeGraphSystem.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** TypeGraphSystem
******************************************************************************/

TypeGraph TypeGraph::__global_instance;
thread_local TypeGraph TypeGraph::__threadlocal_instance;

TypeGraph::TypeGraph()
	: _contents(nullptr)
{
	if (__global_instance._contents != nullptr)
	{
		__global_instance._contents->refcount += 1;
		_contents = __global_instance._contents;
	}

	// this->add(new AnyType());
}
TypeGraph::TypeGraph(TypeGraph const& that)
{

}
TypeGraph::~TypeGraph()
{

}

TypeGraph& TypeGraph::global_instance()
{
	return __global_instance;
}
TypeGraph& TypeGraph::threadlocal_instance()
{
	return __threadlocal_instance;
}

TypeId TypeGraph::anyType() const
{
	return TypeId(1);
}
size_t TypeGraph::typeCount() const
{
	return _contents->types.size();
}

TypeId TypeGraph::add(SType* type)
{
	std::lock_guard<std::recursive_mutex> lock(operation);

	_contents->types.push_back(type);

	return TypeId(_contents->types.size() - 1);
}
SType const* TypeGraph::get(TypeId const& id) const
{
	return _contents->types[id.id - 1];
}

void TypeGraph::import(TypeGraph const& other)
{
	for (auto t : other._contents->types)
	{
		add(t);
	}
}
