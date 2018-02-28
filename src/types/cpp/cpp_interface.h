#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Object
	******************************************************************************/

	/* Virtual base class.
	
	*/
	class Object abstract
		: protected _details::type_impl
	{
	public:
		CRAFT_TYPES_EXPORTED virtual ~Object();
		virtual void* craft_instance_meta() const = 0;
	};

	template <typename TObjectDerived>
	static inline TObjectDerived* craft_static_cast(Object* obj)
	{
		assert(obj != nullptr && "Object is null, use craft_static_cast_try");
		assert(obj->craft_typeId() == TObjectDerived::craft_s_typeId() && "Object not of correct type, cast failed");
		return (TObjectDerived*)obj->craft_instance();
	}

	template <typename TObjectDerived>
	static inline TObjectDerived* craft_static_cast_try(Object* obj)
	{
		if (obj == nullptr) return nullptr;
		if (obj->craft_typeId() != TObjectDerived::craft_s_typeId()) return nullptr;
		return (TObjectDerived*)obj->craft_instance();
	}

	/******************************************************************************
	** type details
	******************************************************************************/

	namespace _details
	{
		template <class TClass>
		class has_craft_s_typeId
		{
			static std::true_type test_signature(TypeId(*)());
			template <class T> static decltype(test_signature(&T::craft_s_typeId)) test(std::nullptr_t);
			template <class T> static std::false_type test(...);
		public:
			using type = decltype(test<TClass>(nullptr));
			static const bool value = type::value;
		};
	}

	/******************************************************************************
	** type
	******************************************************************************/

	template <typename TObject>
	struct type<TObject,
		typename std::enable_if<
			std::is_base_of<Object, TObject>::value
			&& _details::has_craft_s_typeId<TObject>::value
		>::type
	>
		: public type<void>
	{
		static constexpr bool isObject = true;

		inline static TypeId typeId() { return TObject::craft_s_typeId(); }
		inline static std::string name() { return TObject::craft_s_typeName(); }
	};

	template <typename TObject>
	struct type<TObject,
		typename std::enable_if<
			std::is_base_of<Object, TObject>::value
			&& !_details::has_craft_s_typeId<TObject>::value
		>::type
	>
		: public type<void>
	{
	};

	/******************************************************************************
	** Implements
	******************************************************************************/

	template<typename TFeature, typename _ = void>
	struct Implements { };

	/******************************************************************************
	** Implements - Provider
	******************************************************************************/

	template<typename TFeature>
	struct Implements<TFeature,
		typename std::enable_if< std::is_base_of<Provider, TFeature>::value >::type
	> abstract
		: public TFeature
	{
		/******************************************************************************
		** Implements - Provider For
		******************************************************************************/

		template <typename TType>
		class For
			: public TFeature
		{
		protected:
			inline static instance<TType> s_type(instance<void> i)
			{
				return i.asType<TType>();
			}
		public:
			inline For() { }

			inline virtual TypeId craft_featuredTypeId() const override { return ::craft::type<TType>::typeId(); };
		};
	};

	/******************************************************************************
	** Implements - Aspect
	******************************************************************************/

	template<typename TFeature>
	struct Implements<TFeature,
		typename std::enable_if< std::is_base_of<Aspect, TFeature>::value >::type
	> abstract
		: public virtual Object
		, public TFeature
	{
		inline virtual TypeId craft_featuredTypeId() const override { return this->craft_typeId(); }

		inline virtual instance<> craft_featuredInstance() override { return instance<void>(craft_instance(), craft_typeId()); }


		/******************************************************************************
		** Implements - Aspect For
		******************************************************************************/

		template <typename TType>
		class For
			: public TFeature
		{
			instance<TType> _instance;
		protected:
			inline instance<TType>& tance() { return _instance; }
			inline instance<TType> const& tance() const { return _instance; }
		public:
			inline For(instance<TType> inst)
				: _instance(inst)
			{
			}

			inline virtual TypeId craft_featuredTypeId() const override { return type<TType>::typeId(); };

			inline virtual instance<> craft_featuredInstance() override { return _instance; };
		};
	};
}}

#define CRAFT_OBJECT_DECLARE(x) \
    static ::craft::types::_details::type_impl::_static_init __si; \
private: \
    static void __craft_s_types_init(::craft::types::_details::TypeDefineHelper<x> _); \
public: \
    static inline ::craft::types::TypeId craft_s_typeId() { return x::__si.id; } \
	static inline ::std::string craft_s_typeName() { return #x; } \
    inline virtual ::craft::types::TypeId craft_typeId() const override { return craft_s_typeId(); } \
	inline virtual void* craft_instance() const override { return reinterpret_cast<void*>(static_cast<x*>(const_cast<x*>(this))); } \
private: \
    inline ::craft::types::instance<x> craft_instance_from_this() const { return ::craft::types::instance<x>(craft_instance(), craft_typeId()); } \
private:

#define CRAFT_OBJECT_DEFINE(x) \
	::craft::types::_details::type_impl::_static_init x::__si((::craft::types::_details::_fn_register_type_init)&x::__craft_s_types_init); \
	void x::__craft_s_types_init(::craft::types::_details::TypeDefineHelper<x> _)

#define CRAFT_OBJECT_ABSTRACT_DECLARE(x) \
private: \
    template<typename T> friend class ::craft::types::_details::TypeDefineHelper; \
	template<typename T> \
    inline static void __craft_s_types_init(::craft::types::_details::TypeDefineHelper<T> _)

#define CRAFT_TYPE_DECLARE(_dll, x) \
namespace craft { namespace types { \
template <> \
struct type<x> \
	: public type<void> \
	, protected _details::type_impl \
{ \
private: \
	static void s_types_init(_details::ObjectDefineHelper<x> _); \
protected: \
	_dll static _details::type_impl::_static_init __si; \
public: \
	inline static TypeId typeId() { return __si.id; } \
	static constexpr bool isExternal = true; \
}; \
}} \

#define CRAFT_TYPE_DEFINE(x) \
	::craft::types::_details::type_impl::_static_init craft::type<x>::__si((::craft::types::_details::_fn_register_type_init)&::craft::types::type<x>::s_types_init); \
	void ::craft::types::type<x>::s_types_init(::craft::types::_details::ObjectDefineHelper<x> _)
