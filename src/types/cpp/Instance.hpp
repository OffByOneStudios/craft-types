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
		mutable T*					_actual;
		mutable InstanceHeader*		_meta;

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
			: _actual(nullptr)
			, _meta(nullptr)
		{ }

		inline ~instance()
		{
			InstanceHeader::safe_dec(_meta);
		}

		inline instance(instance<T> const& inst)
			: _actual(inst._actual)
			, _meta(InstanceHeader::safe_inc(inst._meta))
		{ }

		inline instance(instance<T> && inst)
			: _actual(nullptr)
			, _meta(nullptr)
		{
			std::swap(_actual, inst._actual);
			std::swap(_meta, inst._meta);
		}

		inline instance<T>& operator=(instance<T> const& _that)
		{
			_actual = _that._actual;
			InstanceHeader::safe_dec(_meta);
			_meta = InstanceHeader::safe_inc(_that._meta);
			return *this;
		}

		inline instance<T>& operator=(instance<T> && _that)
		{
			std::swap(_actual, _that._actual);
			std::swap(_meta, _that._meta);
			return *this;
		}

		// Void instances

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
			, _meta(InstanceHeader::safe_inc(inst._meta))
		{
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
			, _meta(InstanceHeader::safe_inc(inst._meta))
		{
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _actual(inst.getFeature<_T>())
			, _meta(InstanceHeader::safe_inc(inst._meta))
		{
		}

		// From pointers:

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject >::type* = nullptr>
		inline instance(_T* const& ptr)
			: _actual(ptr)
			, _meta(InstanceHeader::safe_inc(static_cast<Object*>(ptr)->craft_header))
		{
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::kind == CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			inline instance(_T* const& ptr)
			: _actual(ptr)
			, _meta(nullptr)
		{
		}

		// From other instance

		template<typename TObject,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature
                && (cpptype<TObject>::isObject || cpptype<TObject>::isRawType)
			>::type* = nullptr >
		inline instance(instance<TObject> const& inst)
			: _actual(inst.template getFeature<_T>())
			, _meta(InstanceHeader::safe_inc(inst._meta))
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

		template<typename TType,
			typename _T = T,
			typename std::enable_if< cpptype<_T>::kind == CppTypeKindEnum::LegacyProvider >::type* = nullptr>
		static inline instance<_T> forType()
		{
			return forType(cpptype<TType>::typeDesc());
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::kind == CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			static inline instance<_T> forType(TypeId type)
		{
			auto actual = type.getFeature<_T>();

			return instance<_T>(actual);
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject  >::type* = nullptr,
			typename... TArgs>
		static inline instance<_T> make(TArgs&&... args)
		{
			_T* actual = new _T(std::forward<TArgs>(args)...);
			actual->craft_setupInstance();

			return instance<_T>(actual);
		}

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr,
			typename... TArgs>
			static inline instance<_T> make(TArgs&&... args)
		{
			_T* actual = new _T(std::forward<TArgs>(args)...);
			InstanceHeader* header = new InstanceHeader(cpptype<_T>::typeDesc(), actual, 0);

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

		inline T* get() { return _actual; }
		inline T const* get() const { return _actual; }

		inline T& value() { return *_actual; }
		inline T const& value() const { return *_actual; }

		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isObject >::type* = nullptr>
			inline bool isNull() const
		{
			return _actual == nullptr;
		}
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isRawType >::type* = nullptr>
			inline bool isNull() const
		{
			return _actual == nullptr;
		}
		template<typename _T = T,
			typename std::enable_if< cpptype<_T>::isLegacyFeature >::type* = nullptr>
			inline bool isNull() const
		{
			return _meta == nullptr || _meta->actual == nullptr;
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
			return typeId().isType<TOtherType>();
		}

		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyProvider >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			return system().typeGetFeature<TFeature>(typeId());
		}

		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyAspect >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			if (this->_meta == nullptr) return nullptr;
			return system().typeGetFeature<TFeature>(instance<>(_meta));
		}

		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			inline bool hasFeature() const
		{
			return system().typeHasFeature<TFeature>(typeId());
		}

		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyAspect >::type* = nullptr>
			inline bool hasFeature() const
		{
			if (this->_meta == nullptr) return nullptr;
			return system().typeHasFeature<TFeature>(instance<>(_meta));
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
			if (_meta == nullptr || cpptype<TType>::typeDesc().asId() != _meta->typeId)
				throw stdext::exception("instance<void>::asType() | T.id != instance.id");
			return instance<TType>(instance<>(_meta));
		}
	};


	//
	// Operators
	//

	template<typename T>
	inline bool operator==(instance<T> const& _this, instance<T> const& _that)
	{
		if (_this._actual == nullptr || _that._actual == nullptr) return _this._actual == _that._actual;
		return _this._actual == _that._actual;
	}

	template<typename T>
	inline bool operator!=(instance<T> const& _this, instance<T> const& _that)
	{ return !(_this == _that); }

	template<typename T>
	inline bool operator<(instance<T> const& _this, instance<T> const& _that)
	{
		if (_that._actual == nullptr) return false; // thAT or both null
		if (_this._actual == nullptr) return true;
		return _this._actual < _that._actual;
	}
	template<typename T>
	inline bool operator>(instance<T> const& _this, instance<T> const& _that)
	{
		if (_this._actual == nullptr) return false; // thIS or both null
		if (_that._actual == nullptr) return true;
		return _this._actual > _that._actual;
	}

	template<typename T>
	inline std::ostream & operator<<(std::ostream & s, instance<T> const & v) { s << v.toString(); return s; }
}}
