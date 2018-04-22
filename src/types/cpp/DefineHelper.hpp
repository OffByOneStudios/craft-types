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
		class DefineHelper_WithFeature
		{
		private:
			DefineHelper_WithFeature()
			{ }

			DefineHelper_WithFeature(DefineHelper_WithFeature const&) = default;

			friend class ::craft::types::CppSystem;
			friend class DefineHelper<TType>;

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
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyProvider>::type* = nullptr>
				inline TConcreate<TType>* byConfiguring()
			{
				auto tc = new TConcreate<TType>();
				system().getManager<TFeature>()->addSingleton(cpptype<TType>::typeDesc(), tc);
				return tc;
			}

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyAspect>::type* = nullptr>
			inline TConcreate<TType>* byConfiguring()
			{
				auto tc = new TConcreate<TType>();
				system().getManager<TFeature>()->addFactory(cpptype<TType>::typeDesc(), tc);
				return tc;
			}
		};

		template<typename TDefine>
		class DefineHelper
		{
		private:
			static_desc* _sd;
			DefineHelper* _parent;

			DefineHelper(static_desc* sd, DefineHelper* parent = nullptr)
				: _sd(sd), _parent(parent)
			{
			}

			DefineHelper(DefineHelper const&) = default;

			friend class ::craft::types::CppSystem;

			inline static void _build_default_providers()
			{
				CppSystem::ensureManager<PIdentifier>();
				CppSystem::ensureManager<PConstructor>();
			}

		public:
			template<typename TInterface>
			inline DefineHelper_WithFeature<TDefine, TInterface>
				use ()
			{
				CppSystem::ensureManager<TInterface>();
				return DefineHelper_WithFeature<TDefine, TInterface>();
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
				TParent::template __craft_s_types_init<TDefine>(TypeDefineHelper(this));
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isObject>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				auto const id = cpptype<TDefine>::typeDesc();
				if (!system().typeHasFeature<PIdentifier>(id)) use<PIdentifier>().template singleton<DefaultIdentifier>();
				if (!system().typeHasFeature<PConstructor>(id)) use<PConstructor>().template singleton<DefaultConstructor>();
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isLegacyFeature>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Instantiate the manager
				CppSystem::ensureManager<TDefine>();

				// Add defaults
				auto const id = cpptype<TDefine>::typeDesc();
				if (!system().typeHasFeature<PIdentifier>(id)) use<PIdentifier>().template singleton<DefaultIdentifier>();
			}

			//
			// MultiMethods
			//
		public:

			// Basic function
			template<
				typename T,
				typename TInfoType_ = TDefine,
				typename std::enable_if< stdext::is_specialization<TInfoType_, cpp::Multimethod>::value >::type* = nullptr>
				inline void add_method(T actual)
			{
				((TInfoType_*)(_sd->repr))->add(actual);
			}

			template<
				typename T,
				typename TTemp,
				typename TInfoType_ = TDefine,
				typename std::enable_if< stdext::is_specialization<TInfoType_, cpp::Multimethod>::value >::type* = nullptr>
				inline void add_method_lambdaHack(TTemp actual)
			{
				T temp = actual;
				((TInfoType_*)(_sd->repr))->add(temp);
			}

		};
	}
}}
