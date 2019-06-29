#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** Symbol
	******************************************************************************/

    struct Symbol final
    {
	private:
        uintptr_t _value;

    public:
        static Symbol Empty;

	public:
		inline Symbol() : _value(0) { }
		inline Symbol(uintptr_t v) : _value(v) { }

		inline operator uintptr_t() const { return _value; }
    };
}
