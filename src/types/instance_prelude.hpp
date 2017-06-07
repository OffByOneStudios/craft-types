#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	namespace _details
	{
		struct InstanceMetaHeader
		{
			InstanceMetaHeader(TypeId tid, void* actual, FeatureId featureId = 0, void* gcInfo = nullptr)
				: typeId(tid)
				, actual(actual)
				, gcInfo(gcInfo)
				, featureId(featureId)
			{

			}

			TypeId typeId;
			void* actual;
			void* gcInfo;
			FeatureId featureId;
		};
	}

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
		mutable void*                            _actual;
		mutable _details::InstanceMetaHeader*    _meta;

		template<typename T, typename T_> friend struct instance;

		template<typename T> friend bool operator==(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator!=(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator<(instance<T> const&, instance<T> const&);
		template<typename T> friend bool operator>(instance<T> const&, instance<T> const&);
	public:
		typedef void instance_type;

	public:
		inline instance()
			: _actual(nullptr)
			, _meta(nullptr)
		{ }

		inline instance(_details::InstanceMetaHeader* meta)
			// By supporting nullptr we get nullptr conversion
			: _actual(meta == nullptr ? nullptr : meta->actual)
			, _meta(meta)
		{ }

		inline instance(void* ptr, TypeId tid)
			: _actual(ptr)
			, _meta(new _details::InstanceMetaHeader(tid, ptr))
		{ }

		template<typename T, typename std::enable_if< type<T>::isObject >::type* = nullptr>
		inline instance(T* ptr)
			: _actual(ptr)
			, _meta(new _details::InstanceMetaHeader(type<T>::typeId(), ptr))
		{ }

		inline TypeId typeId() const { return _meta == nullptr ? 0 : _meta->typeId; }

		template<typename T, typename std::enable_if< std::is_base_of<Object, T>::value >::type>
		inline operator instance<T>() const { return asType<T>(); }

		template<typename T, typename std::enable_if< std::is_base_of<Feature, T>::value && !std::is_base_of<Object, T>::value>::type>
		inline operator instance<T>() const { return getFeature<T>(); }

		inline static instance<void> acquire(Object* o)
		{
			return instance<void>(new _details::InstanceMetaHeader(o->craft_typeId(), o->craft_instance()));
		}

		inline operator bool() const { return !isNull(); }

		inline void* get() { return _actual; }
		inline void const* get() const { return _actual; }

		template<typename T>
		inline instance<T> asType() const
		{
			if (_meta != nullptr && type<T>::typeId() != _meta->typeId)
				throw stdext::exception("instance<void>::asType() | T.id != instance.id");

			return instance<T>(*this);
		}

		template<typename T>
		inline instance<T> asFeature() const
		{
			return instance<T>(*this);
		}

		inline bool isNull() const
		{
			return _actual == nullptr;
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