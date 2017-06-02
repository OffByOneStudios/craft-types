#pragma once
#include "common.h"
#include "core.h"

namespace craft
{
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

		inline virtual instance<> craft_featuredInstance() override { return instance<>::acquire(this); }


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
}

#define CRAFT_TYPE_DECLARE(_dll, x) \
template <> \
struct ::craft::type<x> \
	: public ::craft::type<void> \
	, protected ::craft::_details::type_impl \
{ \
private: \
	static void s_types_init(::craft::_details::ObjectDefineHelper<x> _); \
protected: \
	_dll static ::craft::_details::type_impl::_static_init __si; \
public: \
	inline static TypeId typeId() { return __si.id; } \
	static constexpr bool isExternal = true; \
}; \

#define CRAFT_TYPE_DEFINE(x) \
	::craft::_details::type_impl::_static_init craft::type<x>::__si((::craft::_details::_fn_register_type_init)&::craft::type<x>::s_types_init); \
	void ::craft::type<x>::s_types_init(::craft::_details::ObjectDefineHelper<x> _)
