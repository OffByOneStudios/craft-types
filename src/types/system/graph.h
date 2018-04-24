#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	// This graph will not support import / partial usage. You will have to reimport all of it.
	// Because this graph is not meant to support deletion (replacement only) types have an incrementing index

	/******************************************************************************
	** GraphVector
	******************************************************************************/

	// C friendly vector
	template<typename T>
	class GraphVector
	{
		static_assert(sizeof(T) == sizeof(void*), "Graph vectors only allowed for void* types.");

	public:
		size_t length;
		T* data;

	public:
		inline void push() { };
	};

	/******************************************************************************
	** GraphMeta
	******************************************************************************/

	struct GraphMeta
	{
		enum class Kind : size_t
		{
			Node = 0,
			Prop = 1,
			Edge = 2
		};

		Kind kind;

		// unique/display name of this graph meta type
		//  a stringly typed identifier.
		char const* name;
	};

	/******************************************************************************
	** GraphNodeMeta
	******************************************************************************/

	// This represents the meta interface for nodes
	struct GraphNodeMeta : GraphMeta
	{
	public:

	public:
		static inline GraphNodeMeta* Named(char const* name)
		{
			GraphNodeMeta* ret = new GraphNodeMeta();
			ret->kind = Kind::Node;
			ret->name = name;
			return ret;
		}
	};

	/******************************************************************************
	** GraphPropMeta
	******************************************************************************/

	// This represents the meta interface for properties
	struct GraphPropMeta : GraphMeta
	{
	public:
		enum class Mode : size_t
		{
			Singular = 0,
			Ordered = 1,
			Set = 3,
			Index = (1 << 2),
		};

	public:
		Mode mode;

	public:
		inline bool isSingular() const { return ((size_t)mode & 3) == 0; }
		inline bool isIndex() const { return ((size_t)mode & (1 << 2)) != 0; }

	public:
		static inline GraphPropMeta* Singular(char const* name)
		{
			GraphPropMeta* ret = new GraphPropMeta();
			ret->kind = Kind::Prop;
			ret->name = name;
			ret->mode = Mode::Singular;
			return ret;
		}
	};

	/******************************************************************************
	** IGraphEdgeMeta
	******************************************************************************/

	// This represents the meta interface for edges
	struct GraphEdgeMeta : GraphMeta
	{
	public:
		enum class Mode : size_t
		{
			Generic = 0,
			Directional = 1 << 0,
			Common = 1 << 1, // An optimization that says this is a common edge, and hence should be sorted lower
		};

	public:
		Mode mode;

	public:
		inline bool isDirectional() const { return ((size_t)mode & (1 << 0)) != 0; }
		inline bool isCommon() const { return ((size_t)mode & (1 << 1)) != 0; }

	public:
		static inline GraphEdgeMeta* Directional(char const* name)
		{
			GraphEdgeMeta* ret = new GraphEdgeMeta();
			ret->kind = Kind::Edge;
			ret->name = name;
			ret->mode = Mode::Directional;
			return ret;
		}
	};

	/******************************************************************************
	** Graph
	******************************************************************************/

	class Graph final
	{
	public:
		struct Node;
		struct Prop;
		struct Edge;

		// Anything in the graph fullfills this "interface"
		struct Element
		{
			Node* label;

			Element(Node* label)
				: label(label)
			{ }
		};

		struct Node final : public Element
		{
			void* value;

			std::vector<Prop*> props; // Properties are stored directly
			std::vector<Edge*> edges;

		public:
			Node(Node* label, void* value)
				: Element(label)
				, value(value)
				, props(), edges()
			{ }

		public:
			CRAFT_TYPES_EXPORTED bool isMeta() const;

			CRAFT_TYPES_EXPORTED GraphNodeMeta* getInterface();
		};

		struct Prop final : public Element
		{
			void* value;

		public:
			Prop(Node* label, void* value)
				: Element(label)
				, value(value)
			{ }

		public:
			inline GraphPropMeta* getInterface() { return (GraphPropMeta*)label->value; }
		};

		struct Edge final : public Element
		{
			void* value;
			std::vector<Node*> between;

		public:
			inline GraphEdgeMeta* getInterface() { return (GraphEdgeMeta*)label->value; }
		};

		//
		// Data
		//
	private:
		plf::colony<Node> _nodes;

		std::map<std::string, Node*> _nodeMetas;
		std::map<std::string, Node*> _propMetas;
		std::map<std::string, Node*> _edgeMetas;

		// 
		// Lifecycle
		//
	private:
		Graph(Graph const&) = delete;

	public:
		CRAFT_TYPES_EXPORTED Graph();
		CRAFT_TYPES_EXPORTED ~Graph();

		// 
		// Interface
		//
	public:
		CRAFT_TYPES_EXPORTED Node* ensureMeta(GraphMeta::Kind kind, char const* name, GraphMeta* (*builder)(Node*) = nullptr);

		CRAFT_TYPES_EXPORTED Node* addNode(Node* label, void* value);
		CRAFT_TYPES_EXPORTED void addProp(Node* label, void* value, Node* on_node);
		CRAFT_TYPES_EXPORTED void addEdge(Node* label, void* value, std::vector<Node*> const& edge);

		CRAFT_TYPES_EXPORTED Node* getNodeByValue(void* value);
		CRAFT_TYPES_EXPORTED Prop* getProp(Node* on_node, Node* prop_label);
		CRAFT_TYPES_EXPORTED Edge* getEdgeDirectionalTo(Node* on_node, Node* edge_label, Node* to_node);

		CRAFT_TYPES_EXPORTED std::string dumpNode(Node*);

		// 
		// Templated Interface
		//
	public:
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Node >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Node, T::craftTypes_metaNode_name, (GraphMeta* (*)(Graph::Node*))&T::craftTypes_metaNode_builder); }
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Prop, T::craftTypes_metaProp_name, (GraphMeta* (*)(Graph::Node*))&T::craftTypes_metaProp_builder); }
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Edge >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Edge, T::craftTypes_metaEdge_name, (GraphMeta* (*)(Graph::Node*))&T::craftTypes_metaEdge_builder); }

		template<typename T>
		inline Node* add(typename T::value_type value) { return addNode(meta<T>(), (void*)value); }
		template<typename T>
		inline void add(Node* on_node, typename T::value_type value) { return addProp(meta<T>(), (void*)value, on_node); }
		template<typename T>
		inline void add(typename T::value_type value, std::vector<Node*> const& edge) { return addEdge(meta<T>(), (void*)value, edge); }

		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
		inline typename T::value_type getFirstPropValue(Node* on_node)
		{
			auto p = getProp(on_node, meta<T>());
			return (typename T::value_type)(p != nullptr ? p->value : nullptr);
		}
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
		inline bool hasProp(Node* on_node) { return getProp(on_node, meta<T>()) != nullptr; }

		template<typename TFunc>
		inline void forEachNode(TFunc const& func)
		{
			for (auto it = _nodes.begin(); it != _nodes.end(); ++it)
			{
				func(*this, &*it);
			}
		}

		template<typename TFunc>
		inline void forEachEdgeOnNode(Node* on_node, TFunc const& func)
		{
			for (auto it = on_node->edges.begin(); it != on_node->edges.end(); ++it)
			{
				func(*this, &*it);
			}
		}
	};

	/******************************************************************************
	** GraphPropertyPrinter
	******************************************************************************/

	// Internal graph printer, not meant for use as a "to string" method.
	struct GraphPropertyPrinter final
	{
		// is a   `char const*`
		typedef std::string (*value_type)(void*);
	private:
		GraphPropertyPrinter() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "printer";
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	/******************************************************************************
	** GraphPropertyName
	******************************************************************************/

	struct GraphPropertyName final
	{
		// is a   `char const*`
		typedef char const* value_type;
	private:
		GraphPropertyName() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "name";
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](void* v) -> std::string { return (value_type)v; });
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	/******************************************************************************
	** GraphNodeTypeAbstract
	******************************************************************************/

	struct GraphNodeTypeAbstract final
	{
		// is a   `GraphNodeTypeAbstract*`
		typedef GraphNodeTypeAbstract* value_type;
	public:
		char const* abstract_name;
	private:
		GraphNodeTypeAbstract() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "abstract";
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	/******************************************************************************
	** GraphEdgeIsA
	******************************************************************************/

	struct GraphEdgeIsA final
	{
		// is nothing
		typedef void* value_type;
	private:
		GraphEdgeIsA() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Edge; // needed?
		static constexpr char const* craftTypes_metaEdge_name = "is-a";
		static GraphEdgeMeta* craftTypes_metaEdge_builder(Graph::Node* metanode)
		{
			return GraphEdgeMeta::Directional(craftTypes_metaEdge_name);
		}
	};
}}
