#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** TypeStore
	******************************************************************************/

	class TypeStore final
	{
	private:
		Graph _graph;
		SymbolTable _symbols;

		// 
		// Lifecycle
		//
	private:
		TypeStore(TypeStore const&) = delete;

	public:
		CULTLANG_SYNDICATE_EXPORTED TypeStore();
		CULTLANG_SYNDICATE_EXPORTED ~TypeStore();

		inline Graph& g() { return _graph; }
		inline Graph const& g() const { return _graph; }

		inline SymbolTable& s() { return _symbols; }
		inline SymbolTable const& s() const { return _symbols; }

		CULTLANG_SYNDICATE_EXPORTED std::string describeNode(Graph::Node const*);
	};
}
