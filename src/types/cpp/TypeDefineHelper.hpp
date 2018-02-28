#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	namespace _details
	{
		/* Helper with a specific compiler time feature referenced
		
		*/
		template<typename TType, typename TFeature>
		class TypeDefineHelper_WithFeature
		{
		private:
			Types& _types;

			TypeDefineHelper_WithFeature(Types& types)
				: _types(types)
			{ }

			TypeDefineHelper_WithFeature(TypeDefineHelper_WithFeature const&) = default;

			friend class ::craft::types::Types;
			friend class TypeDefineHelper<TType>;
		public:
			// TODO type check these

			template<class TSingleton>
			inline void singleton()
			{
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					new TSingleton()
				);
			}

			template<template <typename> class TSingleton>
			inline void singleton()
			{
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					new TSingleton<TType>()
				);
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline void singleton(TArgs... args)
			{
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					new TSingleton<TType>(args...)
				);
			}

			template<class TSingleton>
			inline TSingleton* configureSingleton()
			{
				auto res = new TSingleton();
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					res
				);
				return res;
			}

			template<template <typename> class TSingleton>
			inline TSingleton<TType>* configureSingleton()
			{
				auto res = new TSingleton<TType>();
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					res
				);
				return res;
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline TSingleton<TType>* configureSingleton(TArgs... args)
			{
				auto res = new TSingleton<TType>(args...);
				_types.getManager<TFeature>()->addSingleton(
					type<TType>::typeId(),
					res
				);
				return res;
			}

			inline void byCasting()
			{
				_types.getManager<TFeature>()->addFactory(
					type<TType>::typeId(),
					new CastingAspectFactory<TFeature, TType>()
				);
			}

			template<typename TForwarded,
				typename _TType = TType,
				typename std::enable_if<std::is_class<_TType>::value>::type* = nullptr>
			inline void byForwarding(instance<TForwarded> _TType::* mem_ptr)
			{
				_types.getManager<TFeature>()->addFactory(
					type<_TType>::typeId(),
					new ForwardingAspectFactory<TFeature, _TType, TForwarded>(mem_ptr)
				);
			}

		public:

			template<template <typename> class TConcreate>
			inline TConcreate<TType>* byConfiguring()
			{
				auto tc = new TConcreate<TType>();
				_types.getManager<TFeature>()->add(type<TType>::typeId(), TFeature::craft_s_featureId(), tc);
				return tc;
			}
		};

		template<typename TType>
		class TypeDefineHelper
		{
		private:
			Types& _types;
			TypeDefineHelper* _parent;

			TypeDefineHelper(Types& types, TypeDefineHelper* parent = nullptr)
				: _types(types), _parent(parent)
			{
			}

			TypeDefineHelper(TypeDefineHelper const&) = default;

			template <typename T> friend class ObjectDefineHelper;
			friend class ::craft::types::Types;
		public:
			template<typename TInterface>
			inline TypeDefineHelper_WithFeature<TType, TInterface>
				use () { return TypeDefineHelper_WithFeature<TType, TInterface>(_types); }

		public:

			//template<class TParent,
			//	typename std::enable_if< type<TParent>::isObject >::type* = nullptr>
			//inline void parent()
			//{
			//	_types._initers[type<TParent>::typeId()](reinterpret_cast<ObjectDefineHelper<void>&>(ObjectDefineHelper(_types, this)));
			//}
			template<class TParent,
				typename std::enable_if< !(type<TParent>::isObject) >::type* = nullptr>
				inline void parent()
			{
				TParent::template __craft_s_types_init<TType>(TypeDefineHelper(_types, this));
			}

			inline void defaults()
			{
				if (_parent != nullptr) return;

				auto const id = type<TType>::typeId();
				if (!_types.has<PIdentifier>(id)) use<PIdentifier>().template singleton<DefaultIdentifier>();
				if (!_types.has<PConstructor>(id)) use<PConstructor>().template singleton<DefaultConstructor>();
			}
		};
	}
}}
