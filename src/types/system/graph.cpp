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

Graph::Node* Graph::ensureMeta(GraphMeta::Kind kind, char const* name, GraphMeta* (*builder)())
{
	std::map<std::string, Node*>* meta_map;
	switch (kind)
	{
	case GraphMeta::Kind::Node: meta_map = &_nodeMetas; break;
	case GraphMeta::Kind::Prop: meta_map = &_propMetas; break;
	case GraphMeta::Kind::Edge: meta_map = &_edgeMetas; break;
	default: throw stdext::exception("Unknown meta kind {0}", (size_t)kind);
	}

	auto rit = meta_map->find(name);
	if (rit != meta_map->end())
		return rit->second;

	if (builder == nullptr)
		throw stdext::exception("Not passed builder for {0}.", name);
	auto built = builder();

	auto it = _nodes.insert({ nullptr, built });
	meta_map->insert({ name, &*it });

	// add some system properties
	addNode(meta<GraphPropertyName>(), (void*)name);

	// return
	return &*it;
}

Graph::Node* Graph::addNode(Node* label, void* value)
{
	if (label == nullptr)
		throw type_error("Invalid label node.");

	auto it = _nodes.insert({ label, value });

	return &*it;
}

std::string Graph::dumpNode(Node* n) const
{
	if (n->isMeta())
	{
		GraphMeta* meta = (GraphMeta*)n->value;

		return fmt::format("META {0} {1}", (uint8_t)meta->kind, meta->name);
	}

	return "unknown";
}

/******************************************************************************
** Graph::_Node
******************************************************************************/

bool Graph::Node::isMeta() const
{
	return label->value == nullptr;
}

GraphNodeMeta* Graph::Node::getInterface()
{
	assert(!isMeta());
	return (GraphNodeMeta*)label->value;
}
