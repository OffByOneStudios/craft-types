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
	class Function
	{
	public:
		typedef void (*_fnptr)();

		_fnptr _fn;


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
		
		std::tuple<TCallable const*, typename TDispatcher::DispatchRecord const*> dispatchWithRecord(typename TDispatcher::Dispatch const& d) const
		{
			auto res = _dispatcher.dispatchWithRecord(d);

			return std::make_tuple(&((Record*)std::get<0>(res))->callable, std::get<1>(res));
		}
	};
}}
