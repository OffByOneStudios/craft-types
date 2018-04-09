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
	struct Function final
	{
		typedef void (*_fnptr)();

		_fnptr _fn;

		template <typename T
			/*typename std::enable_if< std::is_invocable<T> >::type* = nullptr*/>
		Function(T fn)
		{
			_fn = (_fnptr)fn;
		}
	};

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
		};

		plf::colony<Record> _records;
		TDispatcher _dispatcher;

	public:

		Multimethod()
		{
		}

		void addRecord(typename TDispatcher::DispatchRecord dr, TCallable c)
		{
			auto it = _records.insert({ c });
			_dispatcher.add(dr, (void*)&*it);
		}
		
		std::tuple<TCallable const*, typename TDispatcher::DispatchRecord const*> dispatchWithRecord(typename TDispatcher::Dispatch const& d) const
		{
			auto res = _dispatcher.dispatchWithRecord(d);

			return std::make_tuple(&((Record*)std::get<0>(res))->callable, std::get<1>(res));
		}
	};
}}
