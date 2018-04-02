#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Node
	******************************************************************************/

	struct GraphNode
	{
		void* node_value;
		GraphNode* node_type;

		GraphNode(GraphNode* type, void* value)
			: node_value(value), node_type(type)
		{ }

	protected:
		GraphNode() = default;
		GraphNode(GraphNode const&) = default;
	};

	struct GraphRootNode final
		: GraphNode
	{
	public:
		GraphRootNode(std::string const& s)
			: GraphNode()
		{ }
	};

	/******************************************************************************
	** Edge
	******************************************************************************/

	/******************************************************************************
	** Index
	******************************************************************************/

	/******************************************************************************
	** Graph
	******************************************************************************/

	class Graph sealed
	{
		// 
		// Lifecycle
		//
	private:
		static thread_local Graph* __threadlocal_instance;

		CRAFT_TYPES_EXPORTED Graph(Graph const&);
		CRAFT_TYPES_EXPORTED ~Graph();

	public:
		CRAFT_TYPES_EXPORTED Graph(bool singleton = false);

		CRAFT_TYPES_EXPORTED static Graph& thread_instance();

	public:
	};

	inline Graph& graph() { return Graph::thread_instance(); }
}}
