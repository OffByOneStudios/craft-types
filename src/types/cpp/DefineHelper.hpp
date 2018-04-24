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
			DefineHelper<TType>* _type;
			Graph::Node* _node;

			DefineHelper_WithFeature(DefineHelper<TType>* type)
			{
				_type = type;
				_node = graph().getNodeByValue((void*)cpptype<TFeature>::typeDesc().desc);
			}

			DefineHelper_WithFeature(DefineHelper_WithFeature const&) = default;

			friend class ::craft::types::CppSystem;
			friend class DefineHelper<TType>;

		public:
			// TODO type check these

			template<class TSingleton>
			inline void singleton()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton();

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
			}

			template<template <typename> class TSingleton>
			inline void singleton()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton<TType>();

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline void singleton(TArgs... args)
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton<TType>(args...);

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
			}

			template<class TSingleton>
			inline TSingleton* configureSingleton()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton();

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

				return res;
			}

			template<template <typename> class TSingleton>
			inline TSingleton<TType>* configureSingleton()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton<TType>();

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

				return res;
			}

			template<template <typename> class TSingleton, typename... TArgs>
			inline TSingleton<TType>* configureSingleton(TArgs... args)
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TSingleton<TType>(args...);

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

				return res;
			}

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyProvider>::type* = nullptr>
				inline TConcreate<TType>* byConfiguring()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TConcreate<TType>();

				system().getManager<TFeature>()->addSingleton(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

				return res;
			}

		public:
			inline void byCasting()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new CastingAspectFactory<TFeature, TType>();

				system().getManager<TFeature>()->addFactory(_type->_sd, res );
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
			}

			template<typename TForwarded,
				typename _TType = TType,
				typename std::enable_if<std::is_class<_TType>::value>::type* = nullptr>
			inline void byForwarding(instance<TForwarded> _TType::* mem_ptr)
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new ForwardingAspectFactory<TFeature, _TType, TForwarded>(mem_ptr);

				system().getManager<TFeature>()->addFactory(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });
			}

			template<template <typename> class TConcreate,
				typename _T = TFeature,
				typename std::enable_if<cpptype<_T>::kind == cpp::CppStaticDescKindEnum::LegacyAspect>::type* = nullptr>
			inline TConcreate<TType>* byConfiguring()
			{
				if (_type->_parent != nullptr) throw stdext::exception("adding features with parents not supported");

				auto res = new TConcreate<TType>();

				system().getManager<TFeature>()->addFactory(_type->_sd, res);
				graph().add<GraphEdgeIsA>(nullptr, { _type->_node, _node });
				graph().add<GraphEdgeImplements>(res, { _node, _type->_node });

				return res;
			}
		};

		template<typename TDefine>
		class DefineHelper
		{
		private:
			static_desc* _sd;
			DefineHelper* _parent;

			Graph::Node* _node;

			DefineHelper(static_desc* sd, DefineHelper* parent = nullptr)
				: _sd(sd), _parent(parent)
			{
				if (_parent != nullptr)
					_node = _parent->_node;
				else
					_node = graph().getNodeByValue(sd);
			}

			DefineHelper(DefineHelper const&) = default;

			friend class ::craft::types::CppSystem;
			template<typename TType> friend class DefineHelper;
			template<typename TType, typename TFeature> friend class DefineHelper_WithFeature;

			inline static void _build_default_providers()
			{
				CppSystem::ensureManager<PConstructor>();
			}

		public:
			template<typename TInterface>
			inline DefineHelper_WithFeature<TDefine, TInterface>
				use ()
			{
				CppSystem::ensureManager<TInterface>();
				return DefineHelper_WithFeature<TDefine, TInterface>(this);
			}

			template<typename TGraphMeta
				, typename std::enable_if< TGraphMeta::craftTypes_metaKind == GraphMeta::Kind::Prop >::type* = nullptr>
			void inline add(typename TGraphMeta::value_type value)
			{
				graph().add<TGraphMeta>(_node, value);
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

				return stdext::join('.', _parts.begin(), end);
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
				DefineHelper<void> helper(typeDesc, (DefineHelper<void>*)this);
				typeDesc->initer(helper);
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isObject>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				add<GraphPropertyName>(_T::craft_s_typeName());
				add<GraphPropertyCppName>(_T::craft_s_typeName());
				add<GraphPropertyCppSize>(sizeof(_T));
				add<GraphPropertyTypeName>(new std::string(_cpp_name_to_type_name(_T::craft_s_typeName())));

				if (!system().typeHasFeature<PConstructor>(TypeId(_node))) use<PConstructor>().template singleton<DefaultConstructor>();
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isRawType>::type* = nullptr>
				inline void defaults()
			{
				if (_parent != nullptr) return;

				// Add defaults
				add<GraphPropertyName>(cpptype<_T>::typeName());
				add<GraphPropertyCppName>(cpptype<_T>::typeName());
				add<GraphPropertyCppSize>(sizeof(_T));
				add<GraphPropertyTypeName>(new std::string(_cpp_name_to_type_name(cpptype<_T>::typeName())));
			}

			template<typename _T = TDefine,
				typename std::enable_if<cpptype<_T>::isLegacyFeature>::type* = nullptr>
			inline void defaults()
			{
				if (_parent != nullptr) return;

				// Instantiate the manager
				CppSystem::ensureManager<TDefine>();

				// Add defaults
				add<GraphPropertyName>(_T::craft_s_featureName());
				add<GraphPropertyCppName>(_T::craft_s_typeName());
				add<GraphPropertyCppSize>(sizeof(_T));
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
