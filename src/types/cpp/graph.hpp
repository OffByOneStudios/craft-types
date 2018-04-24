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
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](void* v) -> std::string { return (value_type)v; });
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
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
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
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
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
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
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
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
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
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
		{
			return GraphNodeMeta::Named(craftTypes_metaNode_name);
		}
	};

	struct GraphNodeCppMultiMethod final
	{
		typedef void* value_type;
	private:
		GraphNodeCppMultiMethod() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.multi-method";
		static GraphNodeMeta* craftTypes_metaNode_builder(Graph::Node* metanode)
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
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](void* v) -> std::string { return std::to_string((value_type)v); });
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	struct GraphPropertyCppName final
	{
		// is a   `char const*`
		typedef char const* value_type;
	private:
		GraphPropertyCppName() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "cpp.name";
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			graph().add<GraphPropertyPrinter>(metanode,
				[](void* v) -> std::string { return (value_type)v; });
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
		static GraphEdgeMeta* craftTypes_metaEdge_builder(Graph::Node* metanode)
		{
			return GraphEdgeMeta::Directional(craftTypes_metaEdge_name);
		}
	};
}}
