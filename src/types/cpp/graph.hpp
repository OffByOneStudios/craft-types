#pragma once
#include "types/common.h"
#include "types/core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** system - GraphProperties
	******************************************************************************/

	// Semantic name of the multimethod
	struct GraphPropertyMultiMethodSymbol final
	{
		// is a   `char const*`
		typedef char const* value_type;
	private:
		GraphPropertyMultiMethodSymbol() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "system.multi-method.symbol";
		inline static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](Graph::Element* p) -> std::string { return (value_type)p->value; });
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	/******************************************************************************
	** cpp - GraphNodes
	******************************************************************************/

	struct GraphNodeCppClass final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppClass() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.class";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppObject final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppObject() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.object";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppFeatureLegacy final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppFeatureLegacy() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.feature|legacy";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppFunction final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppFunction() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.function";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppMethod final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppMethod() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.method";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppMultiMethod final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppMultiMethod() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.multi-method";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppUserInfo final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppUserInfo() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.user-info";
		inline static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	/******************************************************************************
	** cpp - GraphProperties
	******************************************************************************/

	struct GraphPropertyCppSize final
	{
		// is a   `size_t`
		typedef size_t value_type;
	private:
		GraphPropertyCppSize() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "cpp.size";
		inline static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](Graph::Element* p) -> std::string { return std::to_string((value_type)p->value); });
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	struct GraphPropertyCppName final
	{
		// is a   `char const*`
		typedef char const* value_type;
		typedef BasicGraphIndex<value_type> index_type;
	private:
		GraphPropertyCppName() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "cpp.name";
		inline static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](Graph::Element* p) -> std::string { return (value_type)p->value; });
			graph().add<GraphPropertyMetaIndex>(metanode, new index_type());
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	/******************************************************************************
	** cpp - GraphEdges
	******************************************************************************/

	struct GraphEdgeImplements final
	{
		// is itself
		typedef void* value_type;
	private:
		GraphEdgeImplements() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Edge; // needed?
		static constexpr char const* craftTypes_metaEdge_name = "cpp.implements|legacy";
		inline static GraphEdgeMeta* craftTypes_metaEdge_builder(Graph::Node* metanode)
		{
			return GraphEdgeMeta::Directional(craftTypes_metaEdge_name);
		}
	};

	struct GraphEdgeInfoPartOfType final
	{
		// is itself
		typedef void* value_type;
	private:
		GraphEdgeInfoPartOfType() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Edge; // needed?
		static constexpr char const* craftTypes_metaEdge_name = "cpp.info-of";
		inline static GraphEdgeMeta* craftTypes_metaEdge_builder(Graph::Node* metanode)
		{
			return GraphEdgeMeta::Directional(craftTypes_metaEdge_name, true);
		}
	};
}}
