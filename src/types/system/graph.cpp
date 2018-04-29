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

Graph::Node* Graph::ensureMeta(GraphMeta::Kind kind, char const* name, GraphMeta* (*builder)(Node*))
{
	// choose meta map
	std::map<std::string, Node*>* meta_map;
	switch (kind)
	{
	case GraphMeta::Kind::Node: meta_map = &_nodeMetas; break;
	case GraphMeta::Kind::Prop: meta_map = &_propMetas; break;
	case GraphMeta::Kind::Edge: meta_map = &_edgeMetas; break;
	default: throw stdext::exception("Unknown meta kind {0}", (size_t)kind);
	}

	// fast exit
	auto rit = meta_map->find(name);
	if (rit != meta_map->end())
		return rit->second;

	// pre-allocate node
	auto node = &*_nodes.insert({ nullptr, nullptr });

	// build it
	if (builder == nullptr)
		throw stdext::exception("Not passed builder for {0}.", name);
	auto built = builder(node);

	node->value = built;
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

	auto index = getFirstPropValue<GraphPropertyMetaIndex>(label);
	if (index != nullptr)
		index->update(index, value, on_node);
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

Graph::Node* Graph::getNodeByValue(void* value)
{
	auto it = std::find_if(
		_nodes.begin(), _nodes.end(),
		[value](auto v) { return v.value == value; });

	if (it == _nodes.end())
		return nullptr;
	return &*it;
}
Graph::Prop* Graph::getProp(Node* on_node, Node* prop_label)
{
	for (auto p : on_node->props)
		if (p->label == prop_label)
			return p;
	return nullptr;
}
Graph::Edge* Graph::getEdgeDirectionalTo(Node* on_node, Node* edge_label, Node* to_node)
{
	for (auto e : on_node->edges)
		if (e->label == edge_label && e->between[1] == to_node)
			return e;
	return nullptr;
}

std::string Graph::dumpNode(Node* n)
{
	std::ostringstream ss;

	// Prime node
	if (n->isMeta())
	{
		GraphMeta* meta = (GraphMeta*)n->value;

		ss << fmt::format("META {0} {1}", (uint8_t)meta->kind, meta->name);
	}
	else if (n->label->isMeta()) // Order 1, TODO order N
	{
		ss << fmt::format("{0}", n->getInterface()->name);
	}
	else
	{
		ss << "unknown";
	}
	ss << std::endl;

	// Properties:
	for (auto p : n->props)
	{
		auto interface = p->getInterface();

		ss << "\t" << interface->name;
		auto printer = getFirstPropValue<GraphPropertyPrinter>(p->label);
		if (printer != nullptr)
			ss << "\t" << printer(p);
		ss << std::endl;
	}

	// Edges: // TODO (use a printer property? rip it out of property?)
	for (auto e : n->edges)
	{
		auto interface = e->getInterface();

		ss << "\t" << interface->name;
		auto printer = getFirstPropValue<GraphPropertyPrinter>(e->label);
		if (printer != nullptr)
			ss << "\t" << printer(e);
		ss << std::endl;

		for (auto n : e->between)
		{
			ss << "\t\t" << n->getInterface()->name;
			auto printer = getFirstPropValue<GraphPropertyPrinter>(n->label);
			if (printer != nullptr)
				ss << "\t" << printer(n);
			else if (hasProp<GraphPropertyName>(n))
				ss << "\t" << getFirstPropValue<GraphPropertyName>(n);
			ss << std::endl;
		}
	}

	// Result
	return ss.str();
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
