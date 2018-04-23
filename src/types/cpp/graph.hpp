#pragma once
#include "types/common.h"
#include "types/core.h"

namespace craft {
namespace types
{
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
		static constexpr char const* craftTypes_metaNode_name = "cpp.type.class";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
	};

	struct GraphNodeCppObject final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppObject() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.type.object";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
	};

	struct GraphNodeCppFeatureLegacy final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppFeatureLegacy() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.type.feature|legacy";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
	};

	struct GraphNodeCppFunction final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppFunction() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.type.function";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
	};

	struct GraphNodeCppMethod final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphNodeCppMethod() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.type.method";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
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
		static GraphPropMeta* craftTypes_metaProp_builder() { return GraphPropMeta::Singular(craftTypes_metaProp_name); }
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
		static GraphPropMeta* craftTypes_metaProp_builder() { return GraphPropMeta::Singular(craftTypes_metaProp_name); }
	};

}}
