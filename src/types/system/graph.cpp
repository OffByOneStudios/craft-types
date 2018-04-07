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

Graph::Node Graph::ensureRoot(std::string const& name)
{
	auto rit = _roots.find(name);
	if (rit != _roots.end())
		return Graph::Node{ const_cast<Graph*>(this), rit->second };

	auto it = _nodes.insert(_Node(nullptr, new RootGraphType(name)));
	_roots.insert({ name, &*it });

	return Graph::Node{ const_cast<Graph*>(this), &*it };
}

Graph::Node Graph::addNode(Node const& label, void* value)
{
	if (!label.isValid())
		throw type_error("Invalid label node.");

	auto it = _nodes.insert(_Node(label._node, value));

	return Graph::Node{ const_cast<Graph*>(this), &*it };
}

Graph::Node Graph::recoverNode(void* ptr)
{
	auto it = _nodes.get_iterator_from_pointer((_Node*)ptr);

	if (it == _nodes.end())
		return Graph::Node{ const_cast<Graph*>(this), nullptr };

	return Graph::Node{ const_cast<Graph*>(this), &*it };
}
