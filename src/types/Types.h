#pragma once
#include "common.h"

namespace craft
{

	// http://stackoverflow.com/questions/3347879/c-downcasting-a-diamond-shape-inherited-object-without-rtti-dynamic-cast

	class Types
	{
	private:
		Types();

	public:
		static CRAFT_TYPE_EXPORTED Types& instance();

	private:
		// Manager Stores
		std::map<FeatureId, IFeatureManager*> _fmsById;
		std::map<std::string, IFeatureManager*> _fmsByName;

		// Type Initializers
		std::map<TypeId, _details::_fn_register_type_init> _initers;
		friend CRAFT_TYPE_EXPORTED void ::craft::_details::_register_type_init(TypeId, _details::_fn_register_type_init);
		template<typename T> friend class ::craft::_details::ObjectDefineHelper;

	protected:
		template<typename TInterface, FeatureKind kind>
		struct impl;

		template<typename TFeature>
		struct impl<TFeature, FeatureKind::Provider>
		{
		private:
			static inline typename TFeature::T_Manager* fm(Types* _this)
			{
				return ((typename TFeature::T_Manager*)_this->_fmsById.at(TFeature::craft_s_featureId()));
			}
		public:
			static inline TFeature* get(Types* _this, TypeId type)
			{
				return (TFeature*)(fm(_this)->getProvider(type));
			}
			static inline TFeature* get(Types* _this, ::craft::instance<> const& inst)
			{
				return get(_this, inst.typeId());
			}

			static inline bool has(Types* _this, TypeId type)
			{
				auto it = _this->_fmsById.find(TFeature::craft_s_featureId());
				return (it != _this->_fmsById.end() && ((typename TFeature::T_Manager*)it->second)->hasProvider(type));
			}
			/*
			static inline void add(Types* _this, TypeId type, TFeature* instance)
			{
				if (_this->_imsById.find(TFeature::craft_s_interfaceId()) == _this->_imsById.end())
				{
					IFeatureManager* n = TFeature::craft_s_interfaceManager();
					assert(n->im_interface() == TFeature::craft_s_interfaceId());
					assert(n->im_kind() == InterfaceKind::Singleton);
					_this->_imsById[TInterface::craft_s_interfaceId()] = n;
					_this->_imsByName[TInterface::craft_c_interfaceName] = n;
				}

				im(_this)->im_add(type, instance);
			}
			template<typename T>
			static inline void add(Types* _this, TFeature* instance)
			{
				add(_this, T::craft_s_typeId(), instance);
			}
			*/
		};

		template<typename TFeature>
		struct impl<TFeature, FeatureKind::Aspect>
		{
		private:
			static inline typename TFeature::T_Manager* fm(Types* _this)
			{
				return ((typename TFeature::T_Manager*)_this->_fmsById.at(TFeature::craft_s_featureId()));
			}
		public:
			// static inline TFeature* get(Types* _this, TypeId type) { /* Not supported by aspects */ }

			static inline TFeature* get(Types* _this, ::craft::instance<> const& inst)
			{
				return (TFeature*)fm(_this)->newAspect(inst.typeId(), (void *)inst.get());
			}

			static inline bool has(Types* _this, TypeId type)
			{
				auto it = _this->_fmsById.find(TFeature::craft_s_featureId());
				return (it != _this->_fmsById.end() && ((typename TFeature::T_Manager*)it->second)->canMakeAspect(type));
			}

			/*
			static inline void add(Types* _this, TypeId type, IInterfaceManagerProviderFactory<TInterface>* instance)
			{
				if (_this->_imsById.find(TInterface::craft_s_interfaceId()) == _this->_imsById.end())
				{
					IFeatureManager* n = TInterface::craft_s_interfaceManager();
					assert(n->im_interface() == TInterface::craft_s_interfaceId());
					assert(n->im_kind() == InterfaceKind::Provider);
					_this->_imsById[TFeature::craft_s_interfaceId()] = n;
					_this->_imsByName[TFeature::craft_c_interfaceName] = n;
				}

				im(_this)->im_add(type, instance);
			}
			template<typename T>
			static inline void add(Types* _this, IInterfaceManagerProviderFactory<TInterface>* instance)
			{
				add(_this, T::craft_s_typeId(), instance);
			}
			*/
		};

	public:
		CRAFT_TYPE_EXPORTED void init();

	public:
		inline IFeatureManager* getManager(FeatureId fid)
		{
			auto it = _fmsById.find(fid);
			if (it == _fmsById.end())
				throw craft::type_error("Types::getManager() | manager not registered");
			return it->second;
		}

		template<typename TFeature>
		inline typename TFeature::T_Manager* getManager()
		{
			FeatureId fid = TFeature::craft_s_featureId();
			try
			{
				return (typename TFeature::T_Manager*)getManager(fid);
			}
			catch (craft::type_error)
			{
				typename TFeature::T_Manager* tman = new typename TFeature::T_Manager();
				_fmsById[fid] = tman;
				return tman;
			}
		}

		template<typename TFeature>
		inline TFeature* get(TypeId type)
		{
			return impl<TFeature, TFeature::craft_c_featureKind>::get(this, type);
		}

		template<typename TFeature>
		inline TFeature* get(::craft::instance<> const& obj)
		{
			return impl<TFeature, TFeature::craft_c_featureKind>::get(this, obj);
		}

		template<typename TFeature>
		inline bool has(TypeId type)
		{
			return impl<TFeature, TFeature::craft_c_featureKind>::has(this, type);
		}

		template<typename TFeature,
			typename std::enable_if<  std::is_base_of<IIndexedProviderManager, typename TFeature::T_Manager>::value  >::type* = nullptr>
		inline TFeature* getIndex(std::string name)
		{
			auto manager = getManager<TFeature>();
			auto tid = manager->index(name);
			if (tid == 0) return nullptr;
			return (TFeature*)manager->getProvider(tid);
		}
	};

	inline Types& types()
	{
		return ::craft::Types::instance();
	}

	namespace _details
	{
		CRAFT_TYPE_EXPORTED void _register_type_init(TypeId id, _fn_register_type_init _init);
	}
}
