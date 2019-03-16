#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** SimpleDispatcher
	******************************************************************************/

	/* Specifically designed for implementing internals, this provides a barebones dispatcher.

	This has some limitations. For starters the dispatch cache lasts as long as the object does. It
	can only really call into cpp function nodes, and it has no concept of qualifiers or affinities
	and will reject them outright.
	*/
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

        typedef void (*FunctionPointer)();

		//
		// Static Helpers
		//
	public:
		CRAFT_TYPES_EXPORTED static void* Any;

		static inline void invokeIntoDispatch(Invoke const& i, Dispatch& d)
		{
			d.reserve(i.size());
			std::transform(i.begin(), i.end(), std::back_inserter(d), (void* const& (*)(InvokeArgument const&))std::get<0>);
		}

		template<typename T,
			typename TActual = typename std::remove_pointer<T>::type,
			typename std::enable_if< cpptype<TActual>::kind != cpp::CppStaticDescKindEnum::None
				&& !(std::is_same<T, void>::value || std::is_same<T, void*>::value || std::is_same<T, uintptr_t>::value) >::type* = nullptr>
		static inline DispatchArgument cppTypeToDispatchArgument()
		{
			return (DispatchArgument)(cpptype<TActual>::typeDesc().asId().node);
		}

		template<typename T,
			typename std::enable_if< std::is_same<T, void*>::value || std::is_same<T, uintptr_t>::value >::type* = nullptr>
			static inline DispatchArgument cppTypeToDispatchArgument()
		{
			return (DispatchArgument)(Any); // any
		}

		template<typename T,
			typename std::enable_if< std::is_same<T, void>::value >::type* = nullptr>
			static inline DispatchArgument cppTypeToDispatchArgument()
		{
			return (DispatchArgument)(nullptr);
		}

		template<typename ...TArgs>
		static inline Dispatch cppTypesToDispatch()
		{
			return Dispatch { cppTypeToDispatchArgument<TArgs>()... };
		}

		template<typename T>
		static inline InvokeArgument cppArgumentToInvokeArgument(T && a)
		{
			return InvokeArgument(cppTypeToDispatchArgument<std::decay<T>::type>(), (void*)a);
		}

		template<typename ...TArgs>
		static inline Invoke cppArgumentsToInvoke(TArgs &&... args)
		{
			return Invoke{ cppArgumentToInvokeArgument<TArgs>(std::forward<TArgs>(args))... };
		}

		template<typename TRet, typename ...TArgs>
		static inline std::tuple<DispatchRecord, FunctionPointer> cppFunctionToRecordAndFunction(TRet (*fn)(TArgs...))
		{
			DispatchRecord record = { cppTypesToDispatch<TArgs...>(), cppTypeToDispatchArgument<TRet>() };
			return std::make_tuple(record, FunctionPointer(fn));
		}

		template<typename T,
			typename std::enable_if< !std::is_pointer<T>::value >::type* = nullptr>
			static inline std::tuple<DispatchRecord, FunctionPointer> cppFunctionToRecordAndFunction(T fn)
		{
			// Decay the lambda
			return cppFunctionToRecordAndFunction(+fn);
		}

		//
		// Invoke Helpers
		//
	public:

		static inline InvokeResult invoke(FunctionPointer f, DispatchRecord const* d, Invoke && i)
		{
			if (d->ret != nullptr)
			{
				switch (i.size())
				{
				case 0:
					return { d->ret, reinterpret_cast<void* (*)()>(f)() };
				case 1:
					return{ d->ret, reinterpret_cast<void* (*)(void*)>(f)(std::get<1>(i[0])) };
				case 2:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1])) };
				case 3:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2])) };
				case 4:
					return{ d->ret, reinterpret_cast<void* (*)(void*, void*, void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2]), std::get<1>(i[3])) };
				default:
					throw type_runtime_error("Simple dispatcher not compiled for this.");
				}
			}
			else
			{
				switch (i.size())
				{
				case 0:
					reinterpret_cast<void(*)()>(f)(); break;
				case 1:
					reinterpret_cast<void (*)(void*)>(f)(std::get<1>(i[0])); break;
				case 2:
					reinterpret_cast<void (*)(void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1])); break;
				case 3:
					reinterpret_cast<void (*)(void*, void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2])); break;
				case 4:
					reinterpret_cast<void (*)(void*, void*, void*, void*)>(f)(std::get<1>(i[0]), std::get<1>(i[1]), std::get<1>(i[2]), std::get<1>(i[3])); break;
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
			FunctionPointer value;
			DispatchRecord dispatch;
		};

		std::vector<_Record> _records;

	public:
		void add(DispatchRecord const& d, FunctionPointer v)
		{
			auto res = dispatchWithRecord(d.args);

			if (std::get<1>(res) != nullptr)
				throw type_runtime_error("Dispatch already exists.");

			_records.push_back({ v, d });
		}

		std::tuple<FunctionPointer, DispatchRecord const*> dispatchWithRecord(Dispatch const& d) const
		{
			auto target_size = d.size();
			for (auto& rec : _records)
			{
				if (rec.dispatch.args.size() != target_size)
					continue;

				bool all_match = true;
				for (size_t i =0; i < target_size; ++i)
				{
					auto dispatching_type = rec.dispatch.args[i];
					if (dispatching_type == Any)
						continue;
					if (dispatching_type != d[i])
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