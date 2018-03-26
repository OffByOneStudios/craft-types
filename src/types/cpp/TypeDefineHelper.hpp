#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	namespace cpp
	{
		/* Helper with a specific compiler time feature referenced
		
		*/
		template<typename TType, typename TFeature>
		class TypeDefineHelper_WithFeature
		{
		private:
			TypeDefineHelper_WithFeature()
			{ }

			TypeDefineHelper_WithFeature(TypeDefineHelper_WithFeature const&) = default;

			friend class ::craft::types::CppSystem;
			friend class TypeDefineHelper<TType>;

		public:
			// TODO type check these

			template<class TSingleton>
			inline void singleton()
			{
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					new TSingleton()
				);
			}

			template<template <typename> class TSingleton>
			inline void singleton()
			{
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					new TSingleton<TType>()
				);
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline void singleton(TArgs... args)
			{
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					new TSingleton<TType>(args...)
				);
			}

			template<class TSingleton>
			inline TSingleton* configureSingleton()
			{
				auto res = new TSingleton();
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					res
				);
				return res;
			}

			template<template <typename> class TSingleton>
			inline TSingleton<TType>* configureSingleton()
			{
				auto res = new TSingleton<TType>();
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					res
				);
				return res;
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline TSingleton<TType>* configureSingleton(TArgs... args)
			{
				auto res = new TSingleton<TType>(args...);
				system().getManager<TFeature>()->addSingleton(
					cpptype<TType>::typeDesc(),
					res
				);
				return res;
			}

			inline void byCasting()
			{
				system().getManager<TFeature>()->addFactory(
					cpptype<TType>::typeDesc(),
					new CastingAspectFactory<TFeature, TType>()
				);
			}

			template<typename TForwarded,
				typename _TType = TType,
				typename std::enable_if<std::is_class<_TType>::value>::type* = nullptr>
			inline void byForwarding(instance<TForwarded> _TType::* mem_ptr)
			{
				system().getManager<TFeature>()->addFactory(
					cpptype<_TType>::typeDesc(),
					new ForwardingAspectFactory<TFeature, _TType, TForwarded>(mem_ptr)
				);
			}

		public:

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppTypeKindEnum::LegacyProvider>::type* = nullptr>
				inline TConcreate<TType>* byConfiguring()
			{
				auto tc = new TConcreate<TType>();
				system().getManager<TFeature>()->addSingleton(cpptype<TType>::typeDesc(), tc);
				return tc;
			}

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppTypeKindEnum::LegacyAspect>::type* = nullptr>
			inline TConcreate<TType>* byConfiguring()
			{
				auto tc = new TConcreate<TType>();
				system().getManager<TFeature>()->addFactory(cpptype<TType>::typeDesc(), tc);
				return tc;
			}
		};

		template<typename TType>
		class TypeDefineHelper
		{
		private:
			type_desc* _td;
			TypeDefineHelper* _parent;

			TypeDefineHelper(type_desc* td, TypeDefineHelper* parent = nullptr)
				: _td(td), _parent(parent)
			{
			}

			TypeDefineHelper(TypeDefineHelper const&) = default;

			friend class ::craft::types::CppSystem;

			inline static void _build_default_providers()
			{
				CppSystem::ensureManager<PIdentifier>();
				CppSystem::ensureManager<PConstructor>();
			}

		public:
			template<typename TInterface>
			inline TypeDefineHelper_WithFeature<TType, TInterface>
				use ()
			{
				CppSystem::ensureManager<TInterface>();
				return TypeDefineHelper_WithFeature<TType, TInterface>();
			}

		public:

			//template<class TParent,
			//	typename std::enable_if< type<TParent>::isObject >::type* = nullptr>
			//inline void parent()
			//{
			//	_types._initers[type<TParent>::typeId()](reinterpret_cast<ObjectDefineHelper<void>&>(ObjectDefineHelper(_types, this)));
			//}
			template<class TParent,
				typename std::enable_if< !(cpptype<TParent>::isObject) >::type* = nullptr>
				inline void parent()
			{
				TParent::template __craft_s_types_init<TType>(TypeDefineHelper(this));
			}

			template<typename _T = TType,
				typename std::enable_if<cpptype<_T>::isObject>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				auto const id = cpptype<TType>::typeDesc();
				if (!system().typeHasFeature<PIdentifier>(id)) use<PIdentifier>().template singleton<DefaultIdentifier>();
				if (!system().typeHasFeature<PConstructor>(id)) use<PConstructor>().template singleton<DefaultConstructor>();
			}

			template<typename _T = TType,
				typename std::enable_if<cpptype<_T>::isLegacyFeature>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Instantiate the manager
				CppSystem::ensureManager<TType>();

				// Add defaults
				auto const id = cpptype<TType>::typeDesc();
				if (!system().typeHasFeature<PIdentifier>(id)) use<PIdentifier>().template singleton<DefaultIdentifier>();
			}
		};
	}
}}
