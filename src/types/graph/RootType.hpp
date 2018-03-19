#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	struct RootType final
	{
		std::string root_kind_name;

		RootType(std::string const& s)
			: root_kind_name(s)
		{ }
	};
}}
