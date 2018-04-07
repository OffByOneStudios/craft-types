#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** ExpressionDispatcher
	******************************************************************************/

	class ExpressionDispatcher final
	{
	public:
		typedef TypeId DispatchArgument;
		typedef std::vector<TypeId> Dispatch;
		struct DispatchRecord
		{
			Dispatch args;
			DispatchArgument ret;
		};

		typedef instance<> InvokeResult;

	public:

		template<typename T>
		static inline instance<> cppArgumentToInvokeArgument(instance<T> && a)
		{
			return a;
		}

		template<typename ...TArgs>
		static inline GenericInvoke cppArgumentsToInvoke(TArgs &&... args)
		{
			return GenericInvoke{ cppArgumentToInvokeArgument<TArgs>(std::forward<TArgs>(args))... };
		}
	};

	/******************************************************************************
	** WithInstancesDispatcher
	******************************************************************************/

	template<typename TTypeDispatcher>
	class WithInstancesDispatcher final
	{
	public:
		typedef typename TTypeDispatcher::DispatchArgument DispatchArgument;
		typedef typename TTypeDispatcher::Dispatch Dispatch;
		typedef typename TTypeDispatcher::DispatchRecord DispatchRecord;
		typedef typename TTypeDispatcher::InvokeResult InvokeResult;
	};
}}