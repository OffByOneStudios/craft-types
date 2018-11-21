#include "../common.h"
#include "../core.h"
#include "graph.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Graph
******************************************************************************/

TypeStore::TypeStore()
{

}
TypeStore::~TypeStore()
{

}

std::string TypeStore::dumpNode(Node* n)
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
** TypeStore
******************************************************************************/

CRAFT_TYPE_DEFINE(craft::types::TypeStore)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Graph::Node> n) -> std::string
	{
		if (graph().hasProp<GraphPropertyTypeName>(n.get()))
			return *graph().getFirstPropValue<GraphPropertyTypeName>(n.get());
		else
			return fmt::format("<<{0}>>", graph().getFirstPropValue<GraphPropertyName>(n.get()));
	});

	_.identify_verbose("craft/types/Graph:Node", "Info", "craft::types::Graph::Node");
	_.defaults();
}

/******************************************************************************
** TypeId
******************************************************************************/

CRAFT_TYPE_DEFINE(craft::types::TypeId)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<Graph::Node> n) -> std::string
	{
		if (graph().hasProp<GraphPropertyTypeName>(n.get()))
			return *graph().getFirstPropValue<GraphPropertyTypeName>(n.get());
		else
			return fmt::format("<<{0}>>", graph().getFirstPropValue<GraphPropertyName>(n.get()));
	});

	_.identify_verbose("craft/types/Graph:Node", "Info", "craft::types::Graph::Node");
	_.defaults();
}

bool Graph::Node::isMeta() const
{
	return label == nullptr;
}

GraphNodeMeta* Graph::Node::getInterface()
{
	assert(!isMeta());
	return (GraphNodeMeta*)label->value;
}
