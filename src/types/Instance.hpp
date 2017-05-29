#pragma once
#include "common.h"
#include "core.h"

/* T:
	This is a collection of classes which provide a struct, similar to `std::shared_ptr` which
	manage refrences to types from within this system.

	Meta data is controlled by the lifecycle provider. Types without lifecycle management cannot
	have metadata, as the meta data might not be properly destructed.
*/

namespace craft
{
	/******************************************************************************
	** Instance Generic, Defines
	******************************************************************************/

	template<typename TFeature>
	TFeature* instance<void>::getFeature() const
	{
		if (this->_actual == nullptr) return nullptr;
		return types().get<TFeature>(*this);
	}

	template<typename TFeature>
	bool instance<void>::hasFeature() const
	{
		return types().has<TFeature>(typeId());
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
			: _actual((_T*)inst._actual)
			, _meta(inst._meta)
		{
		}

		template<typename _T = T,
			typename std::enable_if< type<_T>::isExternal >::type* = nullptr>
			inline instance(instance<void> const& inst)
			: _actual((_T*)inst._actual)
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
                && type<TObject>::isObject
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
			auto actual = types().get<_T>(type);

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

	//
	// Operators
	//
	public:
		inline T& operator* () { return *_actual; }
		inline T* operator-> () { return _actual; }

		inline T const& operator* () const { return *_actual; }
		inline T const* operator-> () const { return _actual; }

		inline operator bool () const { return !isNull(); }

	//
	// Helper Features
	//
	public:
		template<typename TFeature>
		inline TFeature* getFeature() const
		{
			return types().get<TFeature>((instance<> const&)*this);
		}

		template<typename TFeature>
		inline bool hasFeature() const
		{
			return types().has<TFeature>(typeId());
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
	};

	/******************************************************************************
	** Instance Feature, Main
	******************************************************************************/

	/* T:
	This instance type provides access to a typed feature.
	*/

	/*
	template<typename TFeature>
	struct instance<TFeature,
		typename std::enable_if< craft::type<TFeature>::is_feature >::type >
	{
	private:
		mutable _details::InstanceMetaHeader*    _meta;
		mutable TFeature*                        _feature;

		template<typename T, typename T_> friend struct instance;

		template<typename T> friend bool operator==(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator!=(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator<(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator>(instance<T> const&, instance<T> const&);
	//
	// Constructors
	//
	private:
		inline instance(TFeature* feat, TypeId tid)
			: _meta(new _details::InstanceMetaHeader(tid, nullptr))
			, _feature(feat)
		{
		}
	public:

		template<typename TType,
			typename _TFeature = TFeature,
			typename std::enable_if< std::is_base_of<Provider, _TFeature>::value >::type* = nullptr>
		static inline instance<TFeature> make()
		{
			auto actual = types().get<TFeature>(type<TType>::typeId());

			return instance<TFeature>(actual, type<TType>::typeId());
		}

	//
	// Conversions
	//
	public:
		inline operator instance<void>() const { return instance<void>(_meta); }

		template<typename T, typename std::enable_if< std::is_base_of<Object, T>::value >::type>
		inline operator instance<T>() const { return asType<T>(); }


	//
	// Helper Features
	//
	public:
		template<typename TFeatureOther>
		inline TFeatureOther* getFeature() const
		{
			if (TFeature::craft_s_featureId() == TFeature::craft_s_featureId())
				return _feature;
			return types().get<TFeature>(*this);
		}
	};
	*/
	/******************************************************************************
	** Instance Type, Main
	******************************************************************************/

	/* T:
	This instance type provides access to an external type
	*/

	/*

	template<typename TValue>
	struct instance<TValue,
		typename std::enable_if< craft::type<TValue>::isExternal >::type >
	{
	private:
		mutable _details::InstanceMetaHeader*    _meta;
		mutable TValue*                          _value;

		template<typename T, typename T_> friend struct instance;

		template<typename T> friend bool operator==(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator!=(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator<(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator>(instance<T> const&, instance<T> const&);

		//
		// Constructors
		//
	public:
		inline instance()
			: _meta(nullptr)
			, _value(nullptr)
		{
		}

		inline instance(instance<void> const& inst)
			: _meta(inst._meta)
			, _value((TValue*)inst._actual)
		{
		}

		inline instance(TValue* ptr)
			: _meta(new _details::InstanceMetaHeader(type<TValue>::typeId(), ptr))
			, _value(ptr)
		{

		}

		template<typename... TArgs>
		static inline instance<TValue> make(TArgs&&... args)
		{
			TValue* value = new TValue(args...);

			return instance<TValue>(value);
		}

		//
		// Conversions
		//
	public:
		inline operator instance<void>() const
		{
			if (_meta == nullptr) return instance<void>();
			return instance<void>(_meta->actual, _meta);
		}

	};
	*/

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
}
