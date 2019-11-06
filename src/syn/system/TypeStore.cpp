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
				ss << "__meta         :  " << std::hex << TypeId(n->type) << std::endl;
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
				ss << "#Labels     :  " << label_count << std::endl;

	ss << std::endl << "--------------------------------- Edges ----------" << std::endl;
	int edge_count = 0;
	_graph.forAllEdgesOnNode(n, [&](auto* e)
	{
		edge_count += 1;
		void* printer = nullptr;

		if (graph::edgeIsOutgoing<Graph>(n, e)) ss << "   ----- [";
		else if (graph::edgeIsIncoming<Graph>(n, e)) ss << "   <---- [";
		else ss << "   ????? [";
		
		ss << TypeId(e->type) << " ";

		if (printer == nullptr)
			ss << "-" /*"error'Edge Type does not have Edge to printer Implementation'"*/;

		if (graph::edgeIsOutgoing<Graph>(n, e)) ss << "] ---->   ";
		else if (graph::edgeIsIncoming<Graph>(n, e)) ss << "] -----   ";
		else ss << "] ?????   ";


		if (e->nodes.size() == 2)
		{
			if (e->nodes[0] == n) ss << TypeId((Graph::Node*)e->nodes[1]);
			else if (e->nodes[1] == n) ss << TypeId((Graph::Node*)e->nodes[0]);
			else ss << " -?";
		}
		else
		{
			_graph.forAllNodesInEdge(e, [&](auto en)
			{
				if (en != n)
				{
					ss << std::endl << "      - " << TypeId(en->type);
				}
			});
		}
		ss << std::endl;
	});
				ss << "#Edges      :  " << edge_count << std::endl;

	ss << std::endl << "--------------------------------- Properties -----" << std::endl;
	int prop_count = 0;
	_graph.forAllPropsOnNode(n, [&](auto p)
	{
		prop_count += 1;
		void* printer = nullptr;

		ss << "   ." << TypeId(p->type) << " ";

		if (printer == nullptr)
			ss << "-" /*"error'Property Type does not have Edge to printer Implementation'"*/;

		ss << std::endl;
	});
				ss << "#Props      :  " << prop_count << std::endl;

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