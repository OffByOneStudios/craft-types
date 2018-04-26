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
		uintptr_t memInfo;
		void* memManager;
		
		inline InstanceHeader(TypeId tid, void* actual = nullptr, uintptr_t memInfo = 0, void* memManager = nullptr)
			: typeId(tid)
			, actual(actual)
			, memInfo(memInfo)
			, memManager(memManager)
		{

		}

		static inline InstanceHeader* safe_inc(InstanceHeader* this_)
		{
			if (this_ != nullptr && this_->memManager == nullptr)
				this_->memInfo++;
			return this_;
		}
		static inline bool safe_dec(InstanceHeader* this_)
		{
			if (this_ != nullptr && this_->memManager == nullptr)
				return 0 == --(this_->memInfo);
			return false;
		}
	};

	template<size_t TSize>
	struct InstanceHeaderSized
		: InstanceHeader
	{
		unsigned char /*std::byte*/ object[TSize];

		inline InstanceHeaderSized(TypeId tid, void* actual = nullptr, uintptr_t memInfo = 0, void* memManager = nullptr)
			: InstanceHeader(tid, actual, memInfo, memManager) { }
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

	//
	// Core features:
	//
	public:
		inline TypeId typeId() const { return _meta == nullptr ? None : _meta->typeId; }

		inline bool isNull() const
		{
			return _meta == nullptr || _meta->actual == nullptr;
		}

		inline void* get() const { return _meta == nullptr ? nullptr : _meta->actual; }

		inline operator bool() const { return !isNull(); }

	//
	// Pointer features:
	//
	public:
		// Warning, this returns the internal pointer of the instance.
		// You loose all memory management!
		inline void* asInternalPointer() const { return _meta; }

		// Don't forget to decref if you asked for an incref
		static inline instance<void> fromInternalPointer(void* ptr) { return instance<void>((InstanceHeader*)ptr); }

		// Memory leaks are bad, make sure to decref
		inline void incref() { InstanceHeader::safe_inc(_meta); }

		inline void decref() { InstanceHeader::safe_dec(_meta); }

		// DO NOT RELY ON THIS, for diagnostic information ONLY
		inline uintptr_t refCount() const { return _meta->memInfo; }

	//
	// Other Features
	//
	public:
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
