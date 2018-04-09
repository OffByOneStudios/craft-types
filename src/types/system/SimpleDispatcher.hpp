#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** SimpleDispatcher
	******************************************************************************/

	// Specifically for implementing internals
	// Provides barebones features
	class SimpleDispatcher final
	{
		//
		// Types
		//
	public:
		// Graph::Node.ptr()
		typedef void* DispatchArgument;
		typedef std::vector<DispatchArgument> Dispatch;
		struct DispatchRecord
		{
			Dispatch args;
			DispatchArgument ret;
		};

		// 0 is the Graph::Node.ptr()
		// 1 is the instance obeying it
		typedef std::tuple<void*, void*> InvokeArgument;
		typedef InvokeArgument InvokeResult;
		typedef std::vector<InvokeArgument> Invoke;

		//
		// Static Helpers
		//
	public:
		static inline void invokeIntoDispatch(Invoke const& i, Dispatch& d)
		{
			std::transform(i.begin(), i.end(), std::back_inserter(d), (void* const& (*)(InvokeArgument const&))std::get<0>);
		}

		template<typename T,
			typename std::enable_if< std::is_pointer<T>::value >::type* = nullptr>
		static inline DispatchArgument cppTypeToDispatchArgument()
		{
			return (DispatchArgument)(cpptype<typename std::remove_pointer<T>::type>::typeDesc().asNode());
		}

		template<typename ...TArgs>
		static inline Dispatch cppTypesToDispatch()
		{
			return Dispatch { cppTypeToDispatchArgument<TArgs>()... };
		}

		template<typename T>
		static inline InvokeArgument cppArgumentToInvokeArgument(T && a)
		{
			return InvokeArgument(cpptype<T>::typeDesc().asNode(), (void*)a);
		}

		template<typename ...TArgs>
		static inline Invoke cppArgumentsToInvoke(TArgs &&... args)
		{
			return Invoke{ cppArgumentToInvokeArgument<TArgs>(args)... };
		}

		template<typename T
			/*typename std::enable_if< std::is_invocable<T>::value >::type* = nullptr*/>
		static inline std::tuple<DispatchRecord, Function> cppFunctionToRecordAndFunction(T fn)
		{
			// Decay the lambda
			return cppFunctionToRecordAndFunction(+fn);
		}

		template<typename ...TArgs>
		static inline std::tuple<DispatchRecord, Function> cppFunctionToRecordAndFunction(uintptr_t (*fn)(TArgs...))
		{
			DispatchRecord record = { cppTypesToDispatch<TArgs...>(), nullptr };
			return std::make_tuple(record, Function(fn));
		}

		//
		// Invoke Helpers
		//
	public:

		static inline InvokeResult invoke(Function const* f, DispatchRecord const* d, Invoke && i)
		{
			if (d->ret != nullptr)
			{
				switch (i.size())
				{
				case 0:
					return { d->ret, reinterpret_cast<void* (*)()>(f->_fn)() };
				case 1:
					return{ d->ret, reinterpret_cast<void* (*)(void*)>(f->_fn)(std::get<1>(i[0])) };
				case 2:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1])) };
				case 3:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2])) };
				case 4:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*, void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2]), std::get<1>(i[3])) };
				default:
					throw type_runtime_error("Simple dispatcher not compiled for this.");
				}
			}
			else
			{
				switch (i.size())
				{
				case 0:
					reinterpret_cast<void(*)()>(f->_fn)(); break;
				case 1:
					reinterpret_cast<void (*)(void*)>(f->_fn)(std::get<1>(i[0])); break;
				case 2:
					reinterpret_cast<void (*)(void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1])); break;
				case 3:
					reinterpret_cast<void (*)(void*, void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2])); break;
				case 4:
					reinterpret_cast<void (*)(void*, void*, void*, void*)>(f->_fn)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2]), std::get<1>(i[3])); break;
				default:
					throw type_runtime_error("Simple dispatcher not compiled for this.");
				}
				
				return{ nullptr, nullptr };
			}
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
			auto res = dispatchWithRecord(d.args);

			if (std::get<1>(res) != nullptr)
				throw type_runtime_error("Dispatch already exists.");

			_records.push_back({ v, d });
		}

		std::tuple<void*, DispatchRecord const*> dispatchWithRecord(Dispatch const& d) const
		{
			auto target_size = d.size();
			for (auto rec : _records)
			{
				if (rec.dispatch.args.size() != target_size)
					continue;

				bool all_match = true;
				for (auto i = 0; i < target_size; ++i)
				{
					if (rec.dispatch.args[i] != d[i])
					{
						all_match = false;
						break;
					}
				}

				if (all_match)
					return std::make_tuple(rec.value, &rec.dispatch);
			}

			return std::make_tuple(nullptr, nullptr);
		}
	};
}}