#include "../common.h"
#include "../core.h"
#include "store.h"

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
	for (auto s : _strs)
	{
		free((void*)s);
	}
}

std::string TypeStore::dumpNode(Node const* n)
{
	std::ostringstream ss;

	/*
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
	*/
	// Result
	return ss.str();
}

char const* TypeStore::malloc_cstr(char const* s)
{
	if (_strs.find(s) != _strs.end())
		return s;

	char const* new_s = strdup(s);
	_strs.insert(new_s);
	return new_s;
}
char const* TypeStore::malloc_cstr(std::string const& s)
{
	return malloc_cstr(s.c_str());
}

/******************************************************************************
** TypeStore
******************************************************************************/

CRAFT_TYPE_DEFINE(craft::types::TypeStore)
{
	_.identify_byFullCppName("craft::types::TypeStore");
	_.defaults();
}

/******************************************************************************
** TypeId
******************************************************************************/

CRAFT_TYPE_DEFINE(craft::types::TypeId)
{
	_.use<PStringer>().singleton<FunctionalStringer>([](instance<TypeId> n) -> std::string
	{
		return n->toString();
	});

	_.identify_byFullCppName("craft::types::TypeId");
	_.defaults();
}
