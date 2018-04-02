#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Multimethod
	******************************************************************************/

	template<typename TDispatcher>
	class Multimethod final
	{

	};

	/******************************************************************************
	** SimpleTypesDispatcher
	******************************************************************************/

	class SimpleTypesDispatcher final
	{

	};

	/******************************************************************************
	** WithInstancesDispatcher
	******************************************************************************/

	template<typename TTypeDispatcher>
	class WithInstancesDispatcher final
	{

	};
}}
