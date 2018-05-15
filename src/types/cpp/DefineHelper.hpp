#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	namespace cpp
	{
		/******************************************************************************
		** DefineHelper_WithFeature - Declaratino
		******************************************************************************/

		/* Helper with a specific compiler time feature referenced
		
		*/
		template<typename TType, typename TFeature>
		class DefineHelper_WithFeature
		{
		private:
			DefineHelper<TType>* _type;
			Graph::Node* _node;

			DefineHelper_WithFeature(DefineHelper<TType>* type)
			{
				_type = type;
				_node = cpptype<TFeature>::typeDesc().desc->node;

				assert(_node != nullptr);
			}

			DefineHelper_WithFeature(DefineHelper_WithFeature const&) = default;

			friend class ::craft::types::CppSystem;
			friend class DefineHelper<TType>;

		public:
			// TODO type check these

			template<class TSingleton>
			inline void singleton();

			template<template <typename> class TSingleton>
			inline void singleton();

			template<template <typename> class TSingleton, typename... TArgs>
			inline void singleton(TArgs... args);

			template<class TSingleton>
			inline TSingleton* configureSingleton();

			template<template <typename> class TSingleton>
			inline TSingleton<TType>* configureSingleton();

			template<template <typename> class TSingleton, typename... TArgs>
			inline TSingleton<TType>* configureSingleton(TArgs... args);

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyProvider>::type* = nullptr>
			inline TConcreate<TType>* byConfiguring();

		public:
			inline void byCasting();

			template<typename TForwarded,
				typename _TType = TType,
				typename std::enable_if<std::is_class<_TType>::value>::type* = nullptr>
			inline void byForwarding(instance<TForwarded> _TType::* mem_ptr);

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyAspect>::type* = nullptr>
			inline TConcreate<TType>* byConfiguring();
		};

		/******************************************************************************
		** DefineHelper - Specialized Definition
		******************************************************************************/

		template<typename TDefine>
		class DefineHelper
		{
		private:
			static_desc* _sd;
			DefineHelper* _parent;
			void* (*_fn_cast)(void*);

			Graph::Node* _node;

			DefineHelper(static_desc* sd, DefineHelper* parent = nullptr, void* (*fn_cast)(void*) = nullptr)
				: _sd(sd), _parent(parent), _fn_cast(fn_cast)
			{
				if (_parent != nullptr)
					_node = _parent->_node;
				else
					_node = sd->node;

				assert(_node != nullptr);
			}

			DefineHelper(DefineHelper const&) = default;

			friend class ::craft::types::CppSystem;
			template<typename TType> friend class DefineHelper;
			template<typename TType, typename TFeature> friend class DefineHelper_WithFeature;

			inline static void _build_default_providers()
			{
				CppSystem::ensureManager<PConstructor>();
			}

		protected:
			inline std::string _cpp_name_to_type_name(std::string const& s)
			{
				std::string result;

				if (s.find("::") == std::string::npos)
					return "." + s;

				std::vector<std::string> _parts;
				stdext::split(s, "::", std::back_inserter(_parts));
				auto end = std::remove_if(_parts.begin(), _parts.end(), [](std::string i) {
					return i.size() == 0;
				});

				return stdext::join('/', _parts.begin(), end);
			}

			//
			// Graph
			//
		public:

			inline operator Graph::Node*() const { return _node; }

			template<typename TGraphMeta
				, typename std::enable_if< TGraphMeta::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
			void inline add(typename TGraphMeta::value_type value)
			{
				graph().add<TGraphMeta>(_node, value);
			}

			template<typename TGraphMeta
				, typename std::enable_if< TGraphMeta::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
			bool inline has()
			{
				return graph().hasProp<TGraphMeta>(_node);
			}

			//
			// Identifiers
			//
		public:

			void inline identify_asEffectiveCppName(char const* c)
			{
				add<GraphPropertyName>(c);
				add<GraphPropertyCppName>(c);
				// TODO if constexpr (!isLegacyFeature)
				add<GraphPropertyTypeName>(new std::string(_cpp_name_to_type_name(c)));
			}

			void inline identify_verbose(char const* c_name, char const* c_type_name = nullptr, char const* c_cpp_name = nullptr)
			{
				add<GraphPropertyName>(c_name);
				add<GraphPropertyCppName>(c_cpp_name);
				// TODO if constexpr (!isLegacyFeature)
				add<GraphPropertyTypeName>(new std::string(c_type_name));
			}

			//
			// Types
			//
		public:

			//template<class TParent,
			//	typename std::enable_if< type<TParent>::isObject >::type* = nullptr>
			//inline void parent()
			//{
			//	_types._initers[type<TParent>::typeId()](reinterpret_cast<ObjectDefineHelper<void>&>(ObjectDefineHelper(_types, this)));
			//}
			template<class TParent,
				typename std::enable_if<cpptype<TParent>::isObject || cpptype<TParent>::isRawType>::type* = nullptr>
			inline void parent()
			{
				cpp::static_desc* typeDesc = const_cast<cpp::static_desc*>(cpptype<TParent>::typeDesc().desc);
				DefineHelper<void> helper(typeDesc, (DefineHelper<void>*)this,
					+[](void* v) -> void* { return static_cast<TParent*>((TDefine*)v); });
				typeDesc->initer(helper);
				graph().add<GraphEdgeIsA>(nullptr, { _node, typeDesc->node });
			}

			template<typename TInterface>
			inline DefineHelper_WithFeature<TDefine, TInterface> use()
			{
				CppSystem::ensureManager<TInterface>();
				return DefineHelper_WithFeature<TDefine, TInterface>(this);
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isObject>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				if (!has<GraphPropertyName>()) add<GraphPropertyName>(_T::craft_s_typeName());
				if (!has<GraphPropertyCppName>()) add<GraphPropertyCppName>(_T::craft_s_typeName());
				if (!has<GraphPropertyCppSize>()) add<GraphPropertyCppSize>(sizeof(_T));
				if (!has<GraphPropertyTypeName>()) add<GraphPropertyTypeName>(new std::string(_cpp_name_to_type_name(_T::craft_s_typeName())));

				if (!system().typeHasFeature<PConstructor>(TypeId(_node))) use<PConstructor>().template singleton<DefaultConstructor>();
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isRawType>::type* = nullptr>
				inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				if (!has<GraphPropertyName>()) add<GraphPropertyName>(cpptype<_T>::typeName());
				if (!has<GraphPropertyCppName>()) add<GraphPropertyCppName>(cpptype<_T>::typeName());
				if (!has<GraphPropertyCppSize>()) add<GraphPropertyCppSize>(sizeof(_T));
				if (!has<GraphPropertyTypeName>()) add<GraphPropertyTypeName>(new std::string(_cpp_name_to_type_name(cpptype<_T>::typeName())));
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isLegacyFeature>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Instantiate the manager
				CppSystem::ensureManager<TDefine>();

				// Add defaults
				if (!has<GraphPropertyName>()) add<GraphPropertyName>(_T::craft_s_featureName());
				if (!has<GraphPropertyCppName>()) add<GraphPropertyCppName>(_T::craft_s_typeName());
				if (!has<GraphPropertyCppSize>()) add<GraphPropertyCppSize>(sizeof(_T));
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

		/******************************************************************************
		** DefineHelper_WithFeature - Definition
		******************************************************************************/
		template<typename TType, typename TFeature>
		template<class TSingleton>
		inline void DefineHelper_WithFeature<TType, TFeature>::singleton()
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton();

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TSingleton>
		inline void DefineHelper_WithFeature<TType, TFeature>::singleton()
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton<TType>();

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TSingleton, typename... TArgs>
		inline void DefineHelper_WithFeature<TType, TFeature>::singleton(TArgs... args)
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton<TType>(args...);

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
		}

		template<typename TType, typename TFeature>
		template<class TSingleton>
		inline TSingleton* DefineHelper_WithFeature<TType, TFeature>::configureSingleton()
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton();

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

			return res;
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TSingleton>
		inline TSingleton<TType>* DefineHelper_WithFeature<TType, TFeature>::configureSingleton()
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton<TType>();

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

			return res;
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TSingleton, typename... TArgs>
		inline TSingleton<TType>* DefineHelper_WithFeature<TType, TFeature>::configureSingleton(TArgs... args)
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TSingleton<TType>(args...);

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

			return res;
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TConcreate,
			typename _T,
			typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyProvider>::type*>
			inline TConcreate<TType>* DefineHelper_WithFeature<TType, TFeature>::byConfiguring()
		{
			if (_type->_parent != nullptr) throw stdext::exception("adding providers with parents not supported");

			auto res = new TConcreate<TType>();

			system().getManager<TFeature>()->addSingleton(_type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

			return res;
		}

		template<typename TType, typename TFeature>
		inline void DefineHelper_WithFeature<TType, TFeature>::byCasting()
		{
			IAspectFactory<TFeature>* res;
			DefineHelper<void>* type;
			if (_type->_parent != nullptr)
			{
				res = new ParentAspectFactoryWrapper<TFeature, TType, CastingAspectFactory<TFeature, TType>>(_type->_fn_cast);
				type = reinterpret_cast<DefineHelper<void>*>(_type->_parent);
			}
			else
			{
				res = new CastingAspectFactory<TFeature, TType>();
				type = reinterpret_cast<DefineHelper<void>*>(_type);
			}

			system().getManager<TFeature>()->addFactory(type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, type->_node });
		}

		template<typename TType, typename TFeature>
		template<typename TForwarded,
			typename _TType,
			typename std::enable_if<std::is_class<_TType>::value>::type*>
			inline void DefineHelper_WithFeature<TType, TFeature>::byForwarding(instance<TForwarded> _TType::* mem_ptr)
		{
			IAspectFactory<TFeature>* res;
			DefineHelper<void>* type;
			if (_type->_parent != nullptr)
			{
				res = new ParentAspectFactoryWrapper<TFeature, TType, ForwardingAspectFactory<TFeature, _TType, TForwarded>>(_type->_fn_cast, mem_ptr);
				type = reinterpret_cast<DefineHelper<void>*>(_type->_parent);
			}
			else
			{
				res = new ForwardingAspectFactory<TFeature, _TType, TForwarded>(mem_ptr);
				type = reinterpret_cast<DefineHelper<void>*>(_type);
			}

			system().getManager<TFeature>()->addFactory(type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, type->_node });
		}

		template<typename TType, typename TFeature>
		template<template <typename> class TConcreate,
			typename _T,
			typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyAspect>::type*>
			inline TConcreate<TType>* DefineHelper_WithFeature<TType, TFeature>::byConfiguring()
		{
			IAspectFactory<TFeature>* res;
			TConcreate<TType>* ret;
			DefineHelper<void>* type;
			if (_type->_parent != nullptr)
			{
				res = new ParentAspectFactoryWrapper<TFeature, TType, TConcreate<TType>>(_type->_fn_cast);
				ret = &((ParentAspectFactoryWrapper<TFeature, TType, TConcreate<TType>>*)res)->wrapped;
				type = reinterpret_cast<DefineHelper<void>*>(_type->_parent);
			}
			else
			{
				ret = new TConcreate<TType>();
				res = ret;
				type = reinterpret_cast<DefineHelper<void>*>(_type);
			}

			system().getManager<TFeature>()->addFactory(type->_sd, res);
			graph().add<GraphEdgeIsA>(nullptr, { type->_node, _node });
			graph().add<GraphEdgeImplements>(res, { _node, type->_node });

			return ret;
		}
	}
}}
