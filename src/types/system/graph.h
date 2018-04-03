#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** Index
	******************************************************************************/



	/******************************************************************************
	** Graph
	******************************************************************************/

	class Graph final
	{
		//
		// Pieces
		//

		struct _Node;
		struct _Prop;
		struct _Edge;

		friend class Node;

		// Anything in the graph fullfills this "interface"
		struct Element
		{
			_Node* label;
		};

		struct _Node final : public Element
		{
			void* node_value;
			std::vector<_Prop*> properties;
			std::vector<_Edge*> edges;
		};

		struct _Prop final : public Element
		{
			_Node* of_node;
			void* value;
		};

		struct _Edge final : public Element
		{
			void* value;
			std::vector<_Node*> between;
		};

		//
		// Data
		//

		plf::colony<_Node> _nodes;
		plf::colony<_Edge> _edges;
		plf::colony<_Prop> _props;

		// 
		// Lifecycle
		//
	private:
		Graph(Graph const&) = delete;

	public:
		CRAFT_TYPES_EXPORTED Graph();
		CRAFT_TYPES_EXPORTED ~Graph();

		// 
		// Interface Types
		//
	public:

		class Node
		{
		protected:
			friend class Graph;

			Graph* _graph;
			_Node* _node;

			inline Node(Graph* graph, _Node* node)
				: _graph(graph), _node(node)
			{ }

		public:
			inline Node(Node const&) = default;
		};

	public:

		CRAFT_TYPES_EXPORTED Node get(TypeId const&) const;
		CRAFT_TYPES_EXPORTED Node get(cpp::TypePtr const&) const;

		template<typename T>
		inline Node get() { return get(cpptype<T>::typeDesc()); }

		CRAFT_TYPES_EXPORTED Node addNode(Node const& label, void* value) const;
		CRAFT_TYPES_EXPORTED void addProperty(Node const& label, void* value, Node const& on_node) const;
		CRAFT_TYPES_EXPORTED void addEdge(Node const& label, void* value, std::vector<Node> const& edge) const;
	};
}}
