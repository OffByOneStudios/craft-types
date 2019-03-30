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
	Node const* nt = (Node const*)n->type._node;

				ss << "__ptr          :  " << std::hex << n << std::endl;

	if (nt)
		if (auto p = onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(nt))
				ss << "__type         :  " << p->namespace_identifier << std::endl;
		else
				ss << "__type         :  " << "Error[Missing Type_Property_NamespaceIdentifier]" << std::endl;
	else
				ss << "__type         :  " << "Error[NULL]" << std::endl;

	ss << std::endl << "--------------------------------- Identifiers ----" << std::endl;
	if (auto p = onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(n))
				ss << "   .Namespace  :  " << p->namespace_identifier << std::endl;
	if (auto p = onlyPropOfTypeOnNode<Type_Property_LocalIdentifier>(n))
				ss << "   .Local      :  " << p->local_identifier << std::endl;
	if (auto p = onlyPropOfTypeOnNode<Type_Property_CppIdentifier>(n))
				ss << "   .C++        :  " << p->cpp_identifier << std::endl;

	int label_count = 0;
	forAllLabelsOnNode(n, [&](Label const* l)
	{
		label_count += 1;
	});
				ss << "   #Labels     :  " << label_count << std::endl;

	ss << std::endl << "--------------------------------- Edges ----------" << std::endl;
	int edge_count = 0;
	forAllEdgesOnNode(n, [&](Edge const* e)
	{
		edge_count += 1;
		auto type_local_name = onlyPropOfTypeOnNode<Type_Property_LocalIdentifier>((Node const*)e->type._node);
		
		ss << "   >";
		if (type_local_name == nullptr)
			ss << "Unknown";
		else
			ss << type_local_name->local_identifier;

		ss << std::endl;
	});
				ss << "   #Edges      :  " << edge_count << std::endl;

	ss << std::endl << "--------------------------------- Properties -----" << std::endl;
	int prop_count = 0;
	forAllPropsOnNode(n, [&](Prop const* p)
	{
		prop_count += 1;
		auto type_local_name = onlyPropOfTypeOnNode<Type_Property_LocalIdentifier>((Node const*)p->type._node);
		void* printer = nullptr;

		ss << "   .";
		if (type_local_name == nullptr)
			ss << "Unknown";
		else
			ss << type_local_name->local_identifier;

		ss << ":" << std::endl << "        ";

		if (printer == nullptr)
			ss << "Error[Property Type does not have Edge to printer Implementation]";

		ss << std::endl;
	});
				ss << "   #Props      :  " << prop_count << std::endl;

	ss << std::endl;

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
