#pragma once

#include <iostream>

namespace stdext
{
	template<typename TSelf, typename TId>
	struct value_id
	{
		TId id;

		inline value_id() : id(0) { }
		inline value_id(TId const& v) : id(v) { }
		inline explicit operator TId() const { return id; }

		inline TSelf increment() { id++; TId v = id; return v; }

		inline bool operator <(TSelf const& that) const { return this->id < that.id; }
		inline bool operator >(TSelf const& that) const { return this->id > that.id; }
		inline bool operator ==(TSelf const& that) const { return this->id == that.id; }
		inline bool operator !=(TSelf const& that) const { return this->id != that.id; }
	};
	
	template<typename TSelf, typename TId>
	inline std::ostream & operator<<(std::ostream & s, stdext::value_id<TSelf, TId> const & v) { s << v.id; return s; }


	template<typename TSelf, typename TFloat>
	struct value_float
	{
		TFloat value;

		inline value_float() : value(0) { }
		inline value_float(TFloat const& v) : value(v) { }
		inline explicit operator TFloat() const { return value; }

		inline TSelf operator +(TSelf const& that) const { return this->value + that.value; }
		inline TSelf operator -(TSelf const& that) const { return this->value - that.value; }
		inline TSelf operator *(TSelf const& that) const { return this->value * that.value; }
		inline TSelf operator /(TSelf const& that) const { return this->value / that.value; }

		inline TSelf operator +=(TSelf const& that) { return this->value += that.value; }
		inline TSelf operator -=(TSelf const& that) { return this->value -= that.value; }
		inline TSelf operator *=(TSelf const& that) { return this->value *= that.value; }
		inline TSelf operator /=(TSelf const& that) { return this->value /= that.value; }

		inline bool operator <(TSelf const& that) const { return this->value < that.value; }
		inline bool operator >(TSelf const& that) const { return this->value > that.value; }
		inline bool operator ==(TSelf const& that) const { return this->value == that.value; }
		inline bool operator !=(TSelf const& that) const { return this->value != that.value; }
	};

	template<typename TSelf, typename TFloat>
	inline std::ostream & operator<<(std::ostream & s, value_float<TSelf, TFloat> const & v) { s << v.value; return s; }


	template<typename TSelf, typename TInt>
	struct value_int
	{
		TInt value;

		inline value_int() : value(0) { }
		inline value_int(TInt const& v) : value(v) { }
		inline explicit operator TInt() const { return value; }

		inline TSelf operator ++() { return value++; }
		inline TSelf operator ++(int) { TInt v = value; value++; return v; }
		inline TSelf operator --() { return value--; }
		inline TSelf operator --(int) { TInt v = value; value--; return v; }

		inline TSelf operator +(TSelf const& that) const { return this->value + that.value; }
		inline TSelf operator -(TSelf const& that) const { return this->value - that.value; }

		inline TSelf operator +=(TSelf const& that) { return this->value += that.value; }
		inline TSelf operator -=(TSelf const& that) const { return this->value -= that.value; }

		inline bool operator <(TSelf const& that) const { return this->value < that.value; }
		inline bool operator >(TSelf const& that) const { return this->value > that.value; }
		inline bool operator ==(TSelf const& that) const { return this->value == that.value; }
		inline bool operator !=(TSelf const& that) const { return this->value != that.value; }
	};

	template<typename TSelf, typename TInt>
	inline std::ostream & operator<<(std::ostream & s, value_int<TSelf, TInt> const & v) { s << v.value; return s; }
}
