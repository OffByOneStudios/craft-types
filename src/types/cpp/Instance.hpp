#pragma once
#include "../common.h"
#include "../core.h"

/* T:
	This is a collection of classes which provide a struct, similar to `std::shared_ptr` which
	manage refrences to types from within this system.

	Meta data is controlled by the lifecycle provider. Types without lifecycle management cannot
	have metadata, as the meta data might not be properly destructed.
*/

namespace craft {
namespace types
{
	/******************************************************************************
	** Instance Generic, Defines
	******************************************************************************/

	template<typename TFeature>
	TFeature* instance<void>::getFeature() const
	{
		if (this->_meta == nullptr) return nullptr;
		return system().typeGetFeature<TFeature>(*this);
	}

	template<typename TFeature>
	bool instance<void>::hasFeature() const
	{
		if (this->_meta == nullptr) return false;
		return system().typeHasFeature<TFeature>(_meta->typeId);
	}

	/******************************************************************************
	** Instance Object, Main
	******************************************************************************/

	/* T:
		This instance type provides access to a typed instance.
	*/
	template<typename T>
	struct instance<T>
	{

		/* Data section, 2 pointers wide. */
	private:
		mutable InstanceHeader*		_meta;
		mutable T*                  _actual; // Actual should always be assumed null

		template<typename TOther, typename T_> friend struct instance;

		template<typename T_> friend bool operator==(instance<T_> const&, instance<T_> const&);
		template<typename T_> friend bool operator!=(instance<T_> const&, instance<T_> const&);
		template<typename T_> friend bool operator<(instance<T_> const&, instance<T_> const&);
		template<typename T_> friend bool operator>(instance<T_> const&, instance<T_> const&);

	public:
		typedef T instance_type;

	//
	// Constructors
	//
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

		inline instance(instance<T> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(nullptr)
		{ }

		inline instance(instance<T> && inst)
			: _meta(nullptr)
			, _actual(nullptr)
		{
			std::swap(_meta, inst._meta);
		}

		inline instance<T>& operator=(instance<T> const& _that)
		{
			InstanceHeader::safe_dec(_meta);
			_meta = InstanceHeader::safe_inc(_that._meta);
			_actual = nullptr;
			return *this;
		}

		inline instance<T>& operator=(instance<T> && _that)
		{
			std::swap(_meta, _that._meta);
			_actual = nullptr;
			return *this;
		}

		// From generic instance
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
		{
			assert(inst._meta == nullptr || isSubtype(inst._meta->typeId, (TypeId)cpptype<_T>::typeDesc()) && "bad runtime conversion");
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
		{
			assert(inst._meta == nullptr || isSubtype(inst._meta->typeId, (TypeId)cpptype<_T>::typeDesc()) && "bad runtime conversion");
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(inst.getFeature<_T>())
		{
		}


		// From pointers:
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject >::type* = nullptr>
		inline instance(_T* const& ptr)
			: _meta(InstanceHeader::safe_inc(static_cast<Object*>(ptr)->craft_header))
			, _actual(ptr)
		{
			assert(static_cast<Object*>(ptr)->craft_header != nullptr && "object not made correctly, don't use this conversion");
		}

		// From other instance

		template<typename TObject,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature
                && (cpptype<TObject>::isObject || cpptype<TObject>::isRawType)
			>::type* = nullptr >
		inline instance(instance<TObject> const& inst)
			: _meta(InstanceHeader::safe_inc(inst._meta))
			, _actual(inst.template getFeature<_T>())
		{
		}

		template<typename TObject,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature
				&& (cpptype<TObject>::isObject || cpptype<TObject>::isRawType)
			>::type* = nullptr >
		inline instance<T>& operator=(instance<TObject> const& _that)
		{
			_actual = _that.template getFeature<_T>();
			InstanceHeader::safe_dec(_meta);
			_meta = InstanceHeader::safe_inc(_that._meta);
			return *this;
		}

	//
	// make
	//
	public:
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject  >::type* = nullptr,
			typename... TArgs>
		static inline instance<_T> make(TArgs&&... args)
		{
			auto* header = new InstanceHeaderSized<sizeof(T)>(cpptype<T>::typeDesc().asId());
			T* v = new (header->object) _T(std::forward<TArgs>(args)...);
			((Object*)v)->craft_header = header; // TODO, do this before calling new
			header->actual = v;
			v->craft_setupInstance();

			return instance<_T>(header);
		}

		template<typename F,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject  >::type* = nullptr>
		static inline instance<_T> makeThroughLambda(F lambda)
		{
			auto* header = new InstanceHeaderSized<sizeof(T)>(cpptype<T>::typeDesc());
			T* v = lambda(header->object);
			((Object*)v)->craft_header = header;
			header->actual = v;
			v->craft_setupInstance();

			return instance<_T>(header);
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr,
			typename... TArgs>
		static inline instance<_T> make(TArgs&&... args)
		{
			auto* header = new InstanceHeaderSized<sizeof(T)>(cpptype<T>::typeDesc());
			header->actual = new (header->object) _T(std::forward<TArgs>(args)...);

			return instance<_T>(header);
		}

		template<typename F,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType  >::type* = nullptr>
			static inline instance<_T> makeThroughLambda(F lambda)
		{
			auto* header = new InstanceHeaderSized<sizeof(T)>(cpptype<T>::typeDesc());
			lambda(header->object);
			header->actual = header->object;

			return instance<_T>(header);
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr>
			static inline instance<_T> makeFromPointerAndMemoryManager(T* actual, void* memory_manager)
		{
			assert(memory_manager != nullptr);
			
			auto* header = new InstanceHeader(cpptype<T>::typeDesc(), actual, -1, memory_manager);
			return instance<_T>(header);
		}

	//
	// Conversions
	//
	public:
		inline operator instance<void>() const
		{
			return instance<void>( _meta );
		}

	//
	// Core Features
	//
	public:
		inline TypeId typeId() const
		{
			if (_meta == nullptr) return cpptype<T>::typeDesc();
			return _meta->typeId;
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject || cpptype<_T>::isRawType >::type* = nullptr>
		inline void ensureActual() const
		{
			if (_actual == nullptr && _meta != nullptr)
				_actual = (T*)_meta->actual;
		}
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature  >::type* = nullptr>
		inline void ensureActual() const
		{
			if (_actual == nullptr && _meta != nullptr)
				_actual = getFeature<_T>();
		}

		inline T* get() { ensureActual(); return _actual; }
		inline T const* get() const { ensureActual(); return _actual; }

		inline T& value() { ensureActual(); return *_actual; }
		inline T const& value() const { ensureActual(); return *_actual; }

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject || cpptype<_T>::isRawType >::type* = nullptr>
			inline bool isNull() const
		{
			return _meta == nullptr || _meta->actual == nullptr;
		}
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature >::type* = nullptr>
			inline bool isNull() const
		{
			ensureActual();
			return _actual == nullptr;
		}
	//
	// Operators
	//
	public:

		inline T& operator* () { return value(); }
		inline T* operator-> () { return get(); }

		inline T const& operator* () const { return value(); }
		inline T const* operator-> () const { return get(); }

		inline explicit operator bool () const { return !isNull(); }

	//
	// Pointer features:
	//
	public:
		// Warning, this returns the internal pointer of the instance.
		// You loose all memory management!
		inline void* asInternalPointer() const { return _meta; }

		// Don't forget to decref if you asked for an incref
		static inline instance<T> fromInternalPointer(void* ptr) { return instance<T>((InstanceHeader*)ptr); }

		// Memory leaks are bad, make sure to decref
		inline void incref() { InstanceHeader::safe_inc(_meta); }

		inline void decref() { InstanceHeader::safe_dec(_meta); }

		// DO NOT RELY ON THIS, for diagnostic information ONLY
		inline uintptr_t refCount() const { _meta->memInfo; }

	//
	// Stringing
	//
	public:
		inline std::string toString(bool verbose = false) const
		{
			return instance<void>::toString(instance<>(_meta), typeId(), verbose);
		}

	//
	// Helper Features
	//
	public:
		template<typename TOtherType>
		inline bool isType() const
		{
			return typeId().template isType<TOtherType>();
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			return system().template typeGetFeature<TFeature>(typeId());
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyAspect >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			if (this->_meta == nullptr) return nullptr;
			return system().template typeGetFeature<TFeature>(instance<>(_meta));
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
			inline bool hasFeature() const
		{
			return system().template typeHasFeature<TFeature>(typeId());
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyAspect >::type* = nullptr>
			inline bool hasFeature() const
		{
			if (this->_meta == nullptr) return false;
			return system().template typeHasFeature<TFeature>(instance<>(_meta));
		}

		template<typename TFeature>
		inline instance<TFeature> asFeature() const
		{
			return instance<TFeature>(instance<>(_meta));
		}

		template<typename TType,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature
				&& (cpptype<TType>::isObject || cpptype<TType>::isRawType)
			>::type* = nullptr>
		inline instance<TType> asType() const
		{
			if (_meta == nullptr || !isSubtype(_meta->typeId, cpptype<T>::typeDesc().asId()))
				throw stdext::exception("instance<T>::asType() | isSubtype(instance.id, T:id)");
			return instance<TType>(instance<>(_meta));
		}
	};


	//
	// Operators
	//

	template<typename T>
	inline bool operator==(instance<T> const& _this, instance<T> const& _that)
	{
		auto thisp = _this.get(); auto thatp = _that.get();
		if (thisp == nullptr || thatp == nullptr) return thisp == thatp;
		return thisp == thatp;
	}

	template<typename T>
	inline bool operator!=(instance<T> const& _this, instance<T> const& _that)
	{ return !(_this == _that); }

	template<typename T>
	inline bool operator<(instance<T> const& _this, instance<T> const& _that)
	{
		auto thatp = _that.get();
		if (thatp == nullptr) return false; // thAT or both null

		auto thisp = _this.get();
		if (thisp == nullptr) return true;

		return thisp < thatp;
	}
	template<typename T>
	inline bool operator>(instance<T> const& _this, instance<T> const& _that)
	{
		auto thisp = _this.get();
		if (thisp == nullptr) return false; // thIS or both null

		auto thatp = _that.get();
		if (thatp == nullptr) return true;

		return thisp > thatp;
	}

	template<typename T>
	inline std::ostream & operator<<(std::ostream & s, instance<T> const & v) { s << v.toString(); return s; }
}}
