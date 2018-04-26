#pragma once
#include "../common.h"
#include "../core.h"


#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
#endif
namespace craft {
namespace types
{
	template<typename TType = void, typename T_ = void> struct instance;

	/******************************************************************************
	** InstanceHeader
	******************************************************************************/

	struct InstanceHeader
	{
		TypeId typeId;
		void* actual;
		uintptr_t gcInfo;
		// value goes here std::byte[sizeof(T)] see gh#17
		
		InstanceHeader(TypeId tid, void* actual, void* gcInfo = nullptr)
			: typeId(tid)
			, actual(actual)
			, gcInfo((uintptr_t)gcInfo)
		{

		}

		static inline InstanceHeader* safe_inc(InstanceHeader* this_)
		{
			if (this_ != nullptr)
				this_->gcInfo++;
			return this_;
		}
		static inline void safe_dec(InstanceHeader* this_)
		{
			if (this_ != nullptr)
				this_->gcInfo--;
		}
	};

	/******************************************************************************
	** Instance Generic, Declare
	******************************************************************************/

	/* T:
	This instance type provides access to a generic instance.
	*/

	template<>
	struct instance<void>
	{

		/* Data section, 2 pointers wide. */
	private:
		mutable InstanceHeader*     _meta;
		mutable void*               _actual;

	private:
		template<typename T, typename T_> friend struct instance;

		template<typename T> friend bool operator==(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator!=(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator<(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator>(instance<T> const&, instance<T> const&);
	public:
		typedef void instance_type;

	public:
		// Rule of 5 plus default constructor
		inline instance()
			: _meta(nullptr)
			, _actual(nullptr)
		{ }

		inline ~instance()
		{
			InstanceHeader::safe_dec(_meta);
		}

		inline instance(instance<void> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(nullptr)
		{ }

		inline instance(instance<void> && inst)
			: _meta(nullptr)
			, _actual(nullptr)
		{
			std::swap(_meta, inst._meta);
		}

		inline instance<void>& operator=(instance<void> const& _that)
		{
			_meta = InstanceHeader::safe_inc(_that._meta);
			return *this;
		}
		inline instance<void>& operator=(instance<void> && _that)
		{
			std::swap(_meta, _that._meta);
			return *this;
		}

		// Restore constructor
		inline instance(InstanceHeader* meta)
			: _meta(InstanceHeader::safe_inc(meta))
			, _actual(nullptr)
		{ }

		// Other features:
		inline TypeId typeId() const { return _meta == nullptr ? None : _meta->typeId; }

		inline operator bool() const { return !isNull(); }

		inline void* get() const { return _meta == nullptr ? nullptr : _meta->actual; }

		// Warning, this returns the internal pointer of the instance.
		// You loose all memory management!
		inline void* asInternalPointer() const { return _meta; }
		static inline instance<void> fromInternalPointer(void* ptr) { return instance<void>((InstanceHeader*)ptr); }

		template<typename T>
		inline instance<T> asType() const
		{
			assert(_meta != nullptr
				&& cpptype<T>::typeDesc().asId() == _meta->typeId
				&& "instance<void>::asType() | T.id != instance.id");

			return instance<T>(_meta);
		}

		template<typename T>
		inline instance<T> asFeature() const
		{
			return instance<T>(_meta);
		}

		inline bool isNull() const
		{
			return _meta == nullptr || _meta->actual == nullptr;
		}

		inline bool isType(TypeId id) const
		{
			return typeId().isType(id);
		}

		template<typename T>
		inline bool isType() const
		{
			return typeId().isType<T>();
		}

		static CRAFT_TYPES_EXPORTED std::string toString(instance<void> const&, bool verbose = false);
		static CRAFT_TYPES_EXPORTED std::string toString(instance<void> const&, TypeId tid, bool verbose = false);

		inline std::string toString(bool verbose = false) const
		{
			return toString(*this, verbose);
		}

		//
		// Defined in instance.hpp
		//
		template<typename TFeature> inline bool hasFeature() const;
		template<typename TFeature> inline TFeature* getFeature() const;
	};
}

using ::craft::types::instance;

}

#ifdef __clang__
#pragma clang diagnostic pop
#endif
