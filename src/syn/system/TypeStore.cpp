#include "syn/syn.h"
#include "TypeStore.h"

using namespace syn;

using Node = Graph::Node;

/******************************************************************************
** Graph
******************************************************************************/

TypeStore::TypeStore()
{

}
TypeStore::~TypeStore()
{

}

std::string TypeStore::describeNode(Node const* n)
{
	std::ostringstream ss;
	//Node const* nt = (Node const*)n->type.node;

				ss << "id             :  " << std::hex << TypeId(n) << std::endl;
				ss << "__ptr          :  " << std::hex << n << std::endl;

	/*
	if (nt)
		if (auto p = _graph.onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(nt))
				ss << "__type         :  " << p->namespace_identifier << std::endl;
		else
				ss << "__type         :  " << "Error[Missing Type_Property_NamespaceIdentifier]" << std::endl;
	else
				ss << "__type         :  " << "Error[NULL]" << std::endl;

	ss << std::endl << "--------------------------------- Identifiers ----" << std::endl;
	if (auto p = _graph.onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(n))
				ss << "   .Namespace  :  " << p->namespace_identifier << std::endl;
	if (auto p = _graph.onlyPropOfTypeOnNode<Type_Property_LocalIdentifier>(n))
				ss << "   .Local      :  " << p->local_identifier << std::endl;
	if (auto p = _graph.onlyPropOfTypeOnNode<Type_Property_CppIdentifier>(n))
				ss << "   .C++        :  " << p->cpp_identifier << std::endl;
 	*/

	int label_count = 0;
	_graph.forAllLabelsOnNode(n, [&](auto* l)
	{
		label_count += 1;
	});
				ss << "   #Labels     :  " << label_count << std::endl;

	ss << std::endl << "--------------------------------- Edges ----------" << std::endl;
	int edge_count = 0;
	_graph.forAllEdgesOnNode(n, [&](auto e)
	{
		edge_count += 1;
		//auto type_local_name = _graph.firstPropOfTypeOnNode<Type_Property_LocalIdentifier>((Node const*)e->type.node);
		
		ss << "   >";
		//if (type_local_name == nullptr)
			ss << "Unknown";
		//else
		//	ss << type_local_name->local_identifier;

		ss << std::endl;
	});
				ss << "   #Edges      :  " << edge_count << std::endl;

	ss << std::endl << "--------------------------------- Properties -----" << std::endl;
	int prop_count = 0;
	_graph.forAllPropsOnNode(n, [&](auto p)
	{
		prop_count += 1;
		void* printer = nullptr;

		ss << "   ." << TypeId(p->type) << ": ";

		if (printer == nullptr)
			ss << "error'Property Type does not have Edge to printer Implementation'";

		ss << std::endl;
	});
				ss << "   #Props      :  " << prop_count << std::endl;

	ss << std::endl;

	// Result
	return ss.str();
}

/******************************************************************************
** TypeStore
******************************************************************************/

/*
CRAFT_TYPE_DEFINE(syn::TypeStore)
{
	_.identify_byFullCppName("syn::TypeStore");
	_.defaults();
}
*/