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

	auto node = &*_nodes.insert({ nullptr, built });
	meta_map->insert({ name, node });

	// add some system properties
	addProp(meta<GraphPropertyName>(), (void*)name, node);

	// return
	return node;
}

Graph::Node* Graph::addNode(Node* label, void* value)
{
	if (label == nullptr || !label->isMeta()
		|| ((GraphMeta*)label->value)->kind != GraphMeta::Kind::Node)
		throw type_error("Invalid label.");

	auto it = _nodes.insert({ label, value });

	return &*it;
}
void Graph::addProp(Node* label, void* value, Node* on_node)
{
	if (label == nullptr || !label->isMeta()
		|| ((GraphMeta*)label->value)->kind != GraphMeta::Kind::Prop)
		throw type_error("Invalid label.");

	on_node->props.push_back(new Prop { label, value });
}
void Graph::addEdge(Node* label, void* value, std::vector<Node*> const& edges)
{
	if (label == nullptr || !label->isMeta()
		|| ((GraphMeta*)label->value)->kind != GraphMeta::Kind::Edge)
		throw type_error("Invalid label.");

	if (edges.size() == 0)
		throw type_error("Invalid edge.");

	auto edge = new Edge { label, value, edges };

	for (auto edge_node : edges)
		edge_node->edges.push_back(edge);
}

Graph::Prop* Graph::getProp(Node* on_node, Node* prop_label)
{
	for (auto p : on_node->props)
		if (p->label == prop_label)
			return p;
	return nullptr;
}

std::string Graph::dumpNode(Node* n) const
{
	if (n->isMeta())
	{
		GraphMeta* meta = (GraphMeta*)n->value;

		return fmt::format("META {0} {1}", (uint8_t)meta->kind, meta->name);
	}
	else
	{
		// Order 1
		if (n->label->isMeta())
		{
			return fmt::format("{0}", n->getInterface()->name);
		}

		// TODO
	}

	return "unknown";
}

/******************************************************************************
** Graph::_Node
******************************************************************************/

bool Graph::Node::isMeta() const
{
	return label == nullptr;
}

GraphNodeMeta* Graph::Node::getInterface()
{
	assert(!isMeta());
	return (GraphNodeMeta*)label->value;
}
