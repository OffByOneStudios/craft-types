#pragma once
#include "types/common.h"
#include "types/core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** GraphCppStatic
	******************************************************************************/

	struct GraphCppStatic final
	{
		// is a   `cpp::static_desc*`
		typedef cpp::static_desc* value_type;
	private:
		GraphCppStatic() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Node; // needed?
		static constexpr char const* craftTypes_metaNode_name = "cpp.type...";
		static GraphNodeMeta* craftTypes_metaNode_builder() { return GraphNodeMeta::Named(craftTypes_metaNode_name); }
	};
}}
