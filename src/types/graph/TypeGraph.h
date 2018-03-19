#pragma once
#include "../common.h"
#include "../core.h"
#include "graph.h"

namespace craft {
namespace types {

	class TypeGraph final
	{
	private:
		struct _Node;
		struct _Edge;

		struct _Edge
		{
			void* edgePayload;
			TypeId edgePayloadType;

			std::vector<_Node*> nodes;
		};

		struct _Node
		{
			void* nodePayload;
			TypeId nodePayloadType;
			TypeId typeId; // May be none if the content of this node does not have a typeid

			std::vector<_Edge*> edges;
		};

		struct _Data
		{
			std::atomic<int> refcount;
			std::recursive_mutex operation;

			std::vector<_Edge*> _allEdges;
			std::vector<_Node*> _allNodes;
			std::map<TypeId, _Node*> _nodesByType;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	private:
		static TypeGraph __global_instance;
		static thread_local TypeGraph __threadlocal_instance;

		CRAFT_TYPES_EXPORTED TypeGraph();
		CRAFT_TYPES_EXPORTED TypeGraph(TypeGraph const&);
		CRAFT_TYPES_EXPORTED ~TypeGraph();

	public:
		CRAFT_TYPES_EXPORTED static TypeGraph& global_instance();
		CRAFT_TYPES_EXPORTED static TypeGraph& threadlocal_instance();

	//
	// Runtime Interface
	//
	public:
		class Expression
		{
			friend class TypeGraph;

			_Node* _node;

			Expression(_Node* const& c) { _node = c; }
		public:

		};

		inline Expression get(TypeId const& id) const { return Expression { _contents->_nodesByType[id] }; }
	};

	inline TypeGraph& graph()
	{
		return TypeGraph::global_instance();
	}
}}
