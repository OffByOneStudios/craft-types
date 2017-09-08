#pragma once
#include "common.h"
#include "core.h"

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
		if (this->_actual == nullptr) return nullptr;
		return system().get<TFeature>(*this);
	}

	template<typename TFeature>
	bool instance<void>::hasFeature() const
	{
		return system().has<TFeature>(*this);
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
		mutable T*                               _actual;
		mutable _details::InstanceMetaHeader*    _meta;

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
		inline instance()
			: _actual(nullptr)
			, _meta(nullptr)
		{ }

		template<typename _T = T,
			typename std::enable_if< type<_T>::isObject >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
			, _meta(inst._meta)
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isExternal >::type* = nullptr>
			inline instance(instance<void> const& inst)
			: _actual(inst._meta != nullptr ? (_T*)inst._meta->actual : nullptr)
			, _meta(inst._meta)
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isFeature >::type* = nullptr>
		inline instance(instance<void> const& inst)
			: _actual(inst.getFeature<_T>())
			, _meta(new _details::InstanceMetaHeader(inst._meta->typeId, _actual, type<_T>::featureId()))
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isObject >::type* = nullptr>
		inline instance(_T* const& ptr)
			: _actual(ptr)
			, _meta(new _details::InstanceMetaHeader(type<_T>::typeId(), ptr))
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isExternal >::type* = nullptr>
			inline instance(_T* const& ptr)
			: _actual(ptr)
			, _meta(new _details::InstanceMetaHeader(type<_T>::typeId(), ptr))
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isFeature >::type* = nullptr>
			inline instance(_T* const& ptr, TypeId type)
			: _actual(ptr)
			, _meta(new _details::InstanceMetaHeader(type, ptr, ::craft::type<_T>::featureId()))
		{
		}

		/*
		template<typename TObject,
			typename _T = T,
			typename std::enable_if< craft::type<_T>::isFeature
				&& std::is_base_of<Object, TObject>::value >::type* = nullptr >
		inline instance(TObject* const& ptr)
		{
			instance<TObject> inst = ptr;
			_meta = inst._meta;
			_actual = inst.template getFeature<_T>();
		}
		*/

		template<typename TObject,
			typename _T = T,
			typename std::enable_if< type<_T>::isFeature
                && (type<TObject>::isObject || type<TObject>::isExternal)
			>::type* = nullptr >
		inline instance(instance<TObject> const& inst)
			: _actual(inst.template getFeature<_T>())
			, _meta(inst._meta)
		{
		}

		template<typename TType,
			typename _T = T,
			typename std::enable_if< std::is_base_of<Provider, _T>::value >::type* = nullptr>
		static inline instance<_T> forType()
		{
			return forType(type<TType>::typeId());
		}

		template<typename _T = T,
			typename std::enable_if< std::is_base_of<Provider, _T>::value >::type* = nullptr>
			static inline instance<_T> forType(TypeId type)
		{
			auto actual = type.getFeature<_T>();

			return instance<_T>(actual, type);
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isObject  >::type* = nullptr,
			typename... TArgs>
		static inline instance<_T> make(TArgs&&... args)
		{
			_T* actual = new _T(args...);

			return instance<_T>(actual);
		}

		template<typename _T = T,
			typename std::enable_if<  type<_T>::isExternal >::type* = nullptr,
			typename... TArgs>
			static inline instance<_T> make(TArgs&&... args)
		{
			_T* actual = new _T(args...);

			return instance<_T>(actual);
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
		template<typename _T = T,
			typename std::enable_if< type<_T>::isObject >::type* = nullptr>
		inline TypeId typeId() const { return type<_T>::typeId(); }

		template<typename _T = T,
			typename std::enable_if< type<_T>::isExternal >::type* = nullptr>
		inline TypeId typeId() const { return type<_T>::typeId(); }

		template<typename _T = T,
			typename std::enable_if< type<_T>::isFeature >::type* = nullptr>
		inline TypeId typeId() const
		{
			if (_meta == nullptr) return 0;
			return _meta->typeId;
		}

		inline T* get() { return _actual; }
		inline T const* get() const { return _actual; }

		inline T& value() { return *_actual; }
		inline T const& value() const { return *_actual; }

		template<typename _T = T,
			typename std::enable_if< type<_T>::isObject >::type* = nullptr>
			inline bool isNull() const
		{
			return _actual == nullptr;
		}
		template<typename _T = T,
			typename std::enable_if< type<_T>::isExternal >::type* = nullptr>
			inline bool isNull() const
		{
			return _actual == nullptr;
		}
		template<typename _T = T,
			typename std::enable_if< type<_T>::isFeature >::type* = nullptr>
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

		inline operator bool () const { return !isNull(); }

	//
	// Stringing
	//
	public:
		template<typename _T = T,
			typename std::enable_if< !type<_T>::isFeature >::type* = nullptr>
		inline std::string toString(bool verbose = false) const
		{
			return instance<void>::toString(*this, typeId(), verbose);
		}
		template<typename _T = T,
			typename std::enable_if< type<_T>::isFeature >::type* = nullptr>
			inline std::string toString(bool verbose = false) const
		{
			return instance<void>::toString(*this, type<T>::featureId(), verbose);
		}

	//
	// Helper Features
	//
	public:
		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_featureKind != FeatureKind::Aspect >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			return system().get<TFeature>(type<T>::typeId());
		}

		template<typename TFeature,
			typename std::enable_if< TFeature::craft_c_featureKind == FeatureKind::Aspect >::type* = nullptr>
		inline TFeature* getFeature() const
		{
			return system().get<TFeature>((instance<> const&)*this);
		}

		template<typename TFeature>
		inline bool hasFeature() const
		{
			if (isNull())
				return system().has<TFeature>(type<T>::typeId());
			else
				return system().has<TFeature>((instance<> const&)*this);
		}

		template<typename TFeature>
		inline instance<TFeature> asFeature() const
		{
			return instance<TFeature>(*this);
		}

		template<typename TType,
			typename _T = T,
			typename std::enable_if< craft::type<_T>::isFeature
				&& (type<TType>::isObject || type<TType>::isExternal)
			>::type* = nullptr>
		inline instance<TType> asType() const
		{
			if (_meta == nullptr || type<TType>::typeId() != _meta->typeId)
				throw stdext::exception("instance<void>::asType() | T.id != instance.id");
			return instance<TType>((instance<void>)*this);
		}
	};


	//
	// Operators
	//

	template<typename T>
	inline bool operator==(instance<T> const& _this, instance<T> const& _that)
	{
		if (_this._meta == nullptr || _that._meta == nullptr) return _this._meta == _that._meta;
		return _this._meta->actual == _that._meta->actual;
	}

	template<typename T>
	inline bool operator!=(instance<T> const& _this, instance<T> const& _that)
	{ return !(_this == _that); }

	template<typename T>
	inline bool operator<(instance<T> const& _this, instance<T> const& _that)
	{
		if (_that._meta == nullptr) return false; // thAT or both null
		if (_this._meta == nullptr) return true;
		return _this._meta->actual < _that._meta->actual;
	}
	template<typename T>
	inline bool operator>(instance<T> const& _this, instance<T> const& _that)
	{
		if (_this._meta == nullptr) return false; // thIS or both null
		if (_that._meta == nullptr) return true;
		return _this._meta->actual > _that._meta->actual;
	}

	template<typename T>
	inline std::ostream & operator<<(std::ostream & s, instance<T> const & v) { s << v.toString(); return s; }
}}
