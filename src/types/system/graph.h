#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Node
	******************************************************************************/

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
