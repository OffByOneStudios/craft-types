#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** ExpressionDispatcher
	******************************************************************************/

	class ExpressionDispatcher
	{
	public:
		typedef TypeId DispatchArgument;
		typedef std::vector<TypeId> Dispatch;
		typedef ExpressionStore DispatchRecord;

		typedef instance<> InvokeResult;
		typedef instance<> InvokeArgument;
		typedef GenericInvoke Invoke;

	public:

		static inline void invokeIntoDispatch(Invoke const& i, Dispatch& d)
		{
			d.reserve(i.args.size());
			std::transform(i.args.begin(), i.args.end(), std::back_inserter(d), [](auto i) { return i.typeId(); });
		}

		template<typename T,
			typename std::enable_if< stdext::is_specialization<typename std::decay<T>::type, instance>::value >::type* = nullptr>
		static inline instance<> cppArgumentToInvokeArgument(T && a)
		{
			return (instance<>)a;
		}

		template<typename ...TArgs>
		static inline GenericInvoke cppArgumentsToInvoke(TArgs &&... args)
		{
			return GenericInvoke{ cppArgumentToInvokeArgument<TArgs>(std::forward<TArgs>(args))... };
		}

		template<typename T
			/*typename std::enable_if< std::is_invocable<T>::value >::type* = nullptr*/>
		static inline std::tuple<DispatchRecord, void (*)()> cppFunctionToRecordAndFunction(T fn)
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

		//
		// Dispatch manipulators
		//
	public:
		void add(DispatchRecord const& d, void* v)
		{
			_records.push_back({ v, d });
		}

		std::tuple<void*, DispatchRecord const*> dispatchWithRecord(Dispatch const& d) const
		{
			auto expres = ExpressionStore(new ExpressionArrow(to_expression_tuple(d), &ExpressionBottom::Value));

			for (auto& rec : _records)
			{
				if (isSubtype(expres.root(), rec.dispatch.root()))
					return std::make_tuple(rec.value, &rec.dispatch);
			}

			return std::make_tuple(nullptr, nullptr);
		}

		//
		// Invoke Helpers
		//
	public:

		static inline InvokeResult invoke(void (*f)(), DispatchRecord const* d, Invoke && i)
		{
			return types::invoke(*d, *f, i);
		}
	};

	/******************************************************************************
	** WithInstancesDispatcher
	******************************************************************************/

	template<typename TTypeDispatcher>
	class WithInstancesDispatcher
	{
	public:
		typedef typename TTypeDispatcher::DispatchArgument DispatchArgument;
		typedef typename TTypeDispatcher::Dispatch Dispatch;
		typedef typename TTypeDispatcher::DispatchRecord DispatchRecord;
		typedef typename TTypeDispatcher::InvokeResult InvokeResult;
	};


	template<typename TTypeDispatcher>
	class WithCache
		: public TTypeDispatcher
	{
	private:
		mutable std::map<typename TTypeDispatcher::DispatchArgument, std::tuple<void*, typename TTypeDispatcher::DispatchRecord const*>> _cache_1;
		mutable std::map<std::tuple<typename TTypeDispatcher::DispatchArgument, typename TTypeDispatcher::DispatchArgument>, std::tuple<void*, typename TTypeDispatcher::DispatchRecord const*>> _cache_2;

	public:
		std::tuple<void*, typename TTypeDispatcher::DispatchRecord const*> dispatchWithRecord(typename TTypeDispatcher::Dispatch const& d) const
		{
			auto const count = d.size();
			if (count == 1)
			{
				auto const k = d[0];
				auto const lb = _cache_1.lower_bound(k);

				if (lb != _cache_1.end() && !(_cache_1.key_comp()(k, lb->first)))
					return lb->second;
				else
				{
					auto res = TTypeDispatcher::dispatchWithRecord(d);
					if (std::get<1>(res) != nullptr)
						_cache_1.insert(lb, { k, res });
					return res;
				}
			}
			else if (count == 2)
			{
				auto const k = std::make_tuple(d[0], d[1]);
				auto const lb = _cache_2.lower_bound(k);

				if (lb != _cache_2.end() && !(_cache_2.key_comp()(k, lb->first)))
					return lb->second;
				else
				{
					auto res = TTypeDispatcher::dispatchWithRecord(d);
					if (std::get<1>(res) != nullptr)
						_cache_2.insert(lb, { k, res });
					return res;
				}
			}
			else
				return TTypeDispatcher::dispatchWithRecord(d);
		}
	};
}}