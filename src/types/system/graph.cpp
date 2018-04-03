#include "../common.h"
#include "../core.h"
#include "graph.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Graph
******************************************************************************/

Graph::Graph()
{

}
Graph::~Graph()
{

}


Graph::Node Graph::get(TypeId const& tid) const
{
	_Node* n = (_Node*)identifiers().get(tid).node;

	return Graph::Node{ const_cast<Graph*>(this), n };
}
Graph::Node Graph::get(cpp::TypePtr const& tp) const
{
	_Node* n = (_Node*)identifiers().get(tp).node;

	return Graph::Node { const_cast<Graph*>(this), n };
}
