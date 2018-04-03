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
Graph::Node Graph::get(TypeId const&) const
{
	_Node* n = nullptr;

	return Graph::Node { const_cast<Graph*>(this), n };
}
