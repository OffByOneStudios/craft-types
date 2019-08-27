#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** Module
	******************************************************************************/

	class Module final
		: public CppDefine
		, public ModuleBase
	{
	public:
		static constexpr CppDefineKind kind = CppDefineKind::Module;

		inline Module(details::CppDefineRunner<Module> init)
			: CppDefine(kind, this, reinterpret_cast<details::CppDefineRunner<>>(init))
		{ }
	};
}