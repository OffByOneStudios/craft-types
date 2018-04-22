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
	** IGraphNodeMeta
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
	** IGraphPropertyMeta
	******************************************************************************/

	// This represents the meta interface for properties
	struct GraphPropertyMeta : GraphMeta
	{
	public:
		enum class Mode : size_t
		{
			Singular = 0,
			Ordered = 1,
			Set = 3,
			Index = 4,
		};

	public:
		Mode mode;

	public:
		inline bool isSingular() const { return ((size_t)mode & 3) == 0; }
		inline bool isIndex() const { return ((size_t)mode & 4) == 1; }

	public:
		static inline GraphPropertyMeta* Singular(char const* name)
		{
			GraphPropertyMeta* ret = new GraphPropertyMeta();
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

	public:
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

			GraphVector<Prop*> props;
			GraphVector<Edge*> edges;

		public:
			Node(Node* label, void* value)
				: Element(label)
				, value(value)
			{ }

		public:
			CRAFT_TYPES_EXPORTED bool isMeta() const;

			CRAFT_TYPES_EXPORTED GraphNodeMeta* getInterface();
		};

		struct Prop final : public Element
		{
			void* value;

		public:
			inline GraphPropertyMeta* getInterface() { return (GraphPropertyMeta*)label->value; }
		};

		struct Edge final : public Element
		{
			void* value;
			GraphVector<Node*> between;

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
		CRAFT_TYPES_EXPORTED Node* ensureMeta(GraphMeta::Kind kind, char const* name, GraphMeta* (*builder)() = nullptr);

		CRAFT_TYPES_EXPORTED Node* addNode(Node* label, void* value);
		CRAFT_TYPES_EXPORTED void addProp(Node* label, void* value, Node* on_node);
		CRAFT_TYPES_EXPORTED void addEdge(Node* label, void* value, std::vector<Node*> const& edge);

		CRAFT_TYPES_EXPORTED std::string dumpNode(Node*) const;

		// 
		// Templated Interface
		//
	public:
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Node >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Node, T::craftTypes_metaNode_name, (GraphMeta* (*)())&T::craftTypes_metaNode_builder); }
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Prop, T::craftTypes_metaProp_name, (GraphMeta* (*)())&T::craftTypes_metaProp_builder); }
		template<typename T, typename std::enable_if< T::craftTypes_metaKind == GraphMeta::Kind::Edge >::type* = nullptr>
		inline Node* meta() { return ensureMeta(GraphMeta::Kind::Edge, T::craftTypes_metaEdge_name, (GraphMeta* (*)())&T::craftTypes_metaEdge_builder); }

		template<typename T>
		inline Node* add(T* value) { return addNode(meta<T>(), value); }
		template<typename T>
		inline void add(Node* on_node, T* value) { return addProp(meta<T>(), value, on_node); }
		template<typename T>
		inline void add(T* value, std::vector<Node*> const& edge) { return addEdge(meta<T>(), value, edge); }


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
		static GraphPropertyMeta* craftTypes_metaProp_builder() { return GraphPropertyMeta::Singular(craftTypes_metaProp_name); }
	};
}}
