#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	namespace _details
	{
		template<typename TType, typename TFeature>
		class ObjectDefineInterfacedHelper
		{
		private:
			Types& _types;

			ObjectDefineInterfacedHelper(Types& types)
				: _types(types)
			{ }

			ObjectDefineInterfacedHelper(ObjectDefineInterfacedHelper const&) = default;

			friend class ::craft::types::Types;
			friend class ObjectDefineHelper<TType>;
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
		class ObjectDefineHelper
		{
		private:
			Types& _types;
			ObjectDefineHelper* _parent;

			ObjectDefineHelper(Types& types, ObjectDefineHelper* parent = nullptr)
				: _types(types), _parent(parent)
			{
			}

			ObjectDefineHelper(ObjectDefineHelper const&) = default;

			template <typename T> friend class ObjectDefineHelper;
			friend class ::craft::types::Types;
		public:
			template<typename TInterface>
			inline ObjectDefineInterfacedHelper<TType, TInterface>
				use () { return ObjectDefineInterfacedHelper<TType, TInterface>(_types); }

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
				TParent::template __craft_s_types_init<TType>(ObjectDefineHelper(_types, this));
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
