#include "../common.h"
#include "graph.h"
#include "TypeGraph.h"

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

