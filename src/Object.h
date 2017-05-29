#pragma once
#include "common.h"
#include "core.h"

namespace craft
{
	/******************************************************************************
	** Object
	******************************************************************************/

	class Object abstract
		: protected _details::type_impl
	{
	public:
		CRAFT_TYPE_EXPORTED virtual ~Object();
		virtual void* craft_instance() = 0;
		virtual TypeId craft_typeId() const = 0;
	};

	template <typename TObjectDerived>
	static inline TObjectDerived* craft_cast(Object* obj)
	{
		assert(obj != nullptr && "Object is null, use craft_castTry");
		assert(obj->craft_typeId() == TObjectDerived::craft_s_typeId() && "Object not of correct type, cast failed");
		return (TObjectDerived*)obj->craft_instance();
	}

	template <typename TObjectDerived>
	static inline TObjectDerived* craft_castTry(Object* obj)
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
}

#define CRAFT_OBJECT_DECLARE(x) \
private: \
    EXPORTED_MINE static ::craft::_details::type_impl::_static_init __si; \
    static void craft_s_types_init(::craft::_details::ObjectDefineHelper<x> _); \
public: \
    static inline ::craft::TypeId craft_s_typeId() { return x::__si.id; } \
	static inline ::std::string craft_s_typeName() { return #x; } \
    inline virtual ::craft::TypeId craft_typeId() const override { return craft_s_typeId(); } \
	inline virtual void* craft_instance() override { return reinterpret_cast<void*>(static_cast<x*>(this)); } \
private:

#define CRAFT_OBJECT_DEFINE(x) \
	::craft::_details::type_impl::_static_init x::__si((::craft::_details::_fn_register_type_init)&x::craft_s_types_init); \
	void x::craft_s_types_init(::craft::_details::ObjectDefineHelper<x> _)


#define CRAFT_OBJECT_ABSTRACT_DECLARE(x) \
private: \
    template<typename T> friend class ::craft::_details::ObjectDefineHelper; \
	template<typename T> \
    inline static void craft_s_types_init(::craft::_details::ObjectDefineHelper<T> _)