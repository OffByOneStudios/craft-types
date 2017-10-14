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
			InstanceMetaHeader(TypeId tid, void* actual, void* gcInfo = nullptr)
				: typeId(tid)
				, actual(actual)
				, gcInfo((uintptr_t)gcInfo)
			{

			}

			TypeId typeId;
			void* actual;
			uintptr_t gcInfo;

			inline instance<> restore();
			inline InstanceMetaHeader* safe_inc()
			{
				if (this != nullptr)
					gcInfo++;
				return this;
			}
			inline InstanceMetaHeader* safe_dec()
			{
				if (this != nullptr)
					gcInfo--;
				return this;
			}
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
		// Rule of 5 plus default constructor
		inline instance()
			: _actual(nullptr)
			, _meta(nullptr)
		{ }

		inline ~instance()
		{
			_meta->safe_dec();
		}

		inline instance(instance<void> const& inst)
			: _actual(inst._actual)
			, _meta(inst._meta->safe_inc())
		{ }

		inline instance(instance<void> && inst)
			: _actual(nullptr)
			, _meta(nullptr)
		{
			std::swap(_actual, inst._actual);
			std::swap(_meta, inst._meta);
		}

		inline instance<void>& operator=(instance<void> const& _that)
		{
			_actual = _that._actual;
			_meta = _that._meta->safe_inc();
			return *this;
		}
		inline instance<void>& operator=(instance<void> && _that)
		{
			std::swap(_actual, _that._actual);
			std::swap(_meta, _that._meta);
			return *this;
		}

		// Helper constructors
		inline instance(_details::InstanceMetaHeader* meta)
			// By supporting nullptr we get nullptr conversion
			: _actual(meta == nullptr ? nullptr : meta->actual)
			, _meta(meta->safe_inc())
		{ }

		inline instance(void* ptr, TypeId tid)
			: _actual(ptr)
			, _meta((new _details::InstanceMetaHeader(tid, ptr))->safe_inc())
		{ }

		template<typename T, typename std::enable_if< type<T>::isObject >::type* = nullptr>
		inline instance(T* ptr)
			: _actual((void*)ptr)
			, _meta((new _details::InstanceMetaHeader(type<T>::typeId(), ptr))->safe_inc())
		{ }

		// Other features:
		inline TypeId typeId() const { return _meta == nullptr ? (TypeId)0 : _meta->typeId; }

		template<typename T, typename std::enable_if< std::is_base_of<Object, T>::value >::type>
		inline operator instance<T>() const { return asType<T>(); }

		template<typename T, typename std::enable_if< std::is_base_of<Feature, T>::value && !std::is_base_of<Object, T>::value>::type>
		inline operator instance<T>() const { return asFeature(); }

		inline operator bool() const { return !isNull(); }

		inline void* get() { return _actual; }
		inline void const* get() const { return _actual; }

		template<typename T>
		inline instance<T> asType() const
		{
			if (_meta != nullptr && type<T>::typeId() != _meta->typeId)
				throw stdext::exception("instance<void>::asType() | T.id != instance.id");

			return instance<T>(_meta->restore());
		}

		template<typename T>
		inline instance<T> asFeature() const
		{
			return instance<T>(_meta->restore());
		}

		inline bool isNull() const
		{
			return _actual == nullptr;
		}

		inline bool isFinal() const
		{
			return _actual == _meta->actual;
		}

		static CRAFT_TYPE_EXPORTED std::string toString(instance<void> const&, bool verbose = false);
		static CRAFT_TYPE_EXPORTED std::string toString(instance<void> const&, TypeId tid, bool verbose = false);
		static CRAFT_TYPE_EXPORTED std::string toString(instance<void> const&, FeatureId fid, bool verbose = false);

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

	inline instance<> _details::InstanceMetaHeader::restore()
	{
		return instance<void>(this);
	}
}

using ::craft::types::instance;

}
