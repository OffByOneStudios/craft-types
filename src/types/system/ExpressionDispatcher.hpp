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
		typedef ExpressionStore DispatchRecord;

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

		template<typename T
			/*typename std::enable_if< std::is_invocable<T>::value >::type* = nullptr*/>
			static inline std::tuple<DispatchRecord, Function> cppFunctionToRecordAndFunction(T fn)
		{
			return to_expression_and_function(fn);
		}

		//
		// Actual Class Data
		//
	private:
		struct _Record
		{
			void* value;
			DispatchRecord dispatch;
		};

		std::vector<_Record> _records;

	public:
		void add(DispatchRecord const& d, void* v)
		{
			// TODO
		}

		std::tuple<void*, DispatchRecord const*> dispatchWithRecord(Dispatch const& d) const
		{
			// TODO
			throw type_graph_not_implemented_error("requires typegraph");
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