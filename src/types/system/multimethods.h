#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Function
	******************************************************************************/

	// TODO, speed this up later


	/******************************************************************************
	** Multimethod
	******************************************************************************/

	template<typename TCallable, typename TDispatcher>
	class Multimethod
	{
	private:
		struct Record
		{
			TCallable callable;
			typename TDispatcher::DispatchRecord dispatch;
		};

	public:

		CRAFT_TYPES_EXPORTED TCallable const& dispatch(typename TDispatcher::DispatchInvoke);
	};

	/******************************************************************************
	** SimpleTypesDispatcher
	******************************************************************************/

	class SimpleTypesDispatcher final
	{
	public:
		typedef int DispatchRecord;
		typedef int DispatchInvoke;
	};

	/******************************************************************************
	** WithInstancesDispatcher
	******************************************************************************/

	template<typename TTypeDispatcher>
	class WithInstancesDispatcher final
	{

	};
}}
