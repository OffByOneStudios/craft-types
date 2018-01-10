#pragma once
#include "common.h"

namespace craft {
namespace types
{

	// http://stackoverflow.com/questions/3347879/c-downcasting-a-diamond-shape-inherited-object-without-rtti-dynamic-cast

	class Types
	{
	private:
		Types();

	public:
		static CRAFT_TYPES_EXPORTED Types& instance();

	private:
		//
		// Storage
		//

		// Feature Manager Storage
		std::map<FeatureId, IFeatureManager*> _fmsById;
		std::map<std::string, IFeatureManager*> _fmsByName;

		//
		// Initializers
		//

		// Type Initializers
		std::map<TypeId, _details::_fn_register_type_init> _initersForTypes;
		friend CRAFT_TYPES_EXPORTED void ::craft::types::_details::_register_type_init(TypeId, _details::_fn_register_type_init);
		template<typename T> friend class ::craft::types::_details::ObjectDefineHelper;

		// Feature Initializers
		std::map<FeatureId, _details::_fn_register_feature_init> _initersForFeatures;
		friend CRAFT_TYPES_EXPORTED void ::craft::types::_details::_register_feature_init(FeatureId, _details::_fn_register_feature_init);

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
			static inline bool has(Types* _this, ::craft::instance<> const& inst)
			{
				return has(_this, inst.typeId());
			}
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
				return (TFeature*)fm(_this)->getAspect(inst.typeId(), (void *)inst.get());
			}

			static inline bool has(Types* _this, TypeId type)
			{
				auto it = _this->_fmsById.find(TFeature::craft_s_featureId());
				return (it != _this->_fmsById.end() && ((typename TFeature::T_Manager*)it->second)->hasAspect(type, nullptr));
			}

			static inline bool has(Types* _this, ::craft::instance<> const& inst)
			{
				auto it = _this->_fmsById.find(TFeature::craft_s_featureId());
				return (it != _this->_fmsById.end() && ((typename TFeature::T_Manager*)it->second)->hasAspect(inst.typeId(), (void *)inst.get()));
			}
		};

	public:
		CRAFT_TYPES_EXPORTED void init();

	public:
		inline IFeatureManager* getManager(FeatureId fid)
		{
			auto it = _fmsById.find(fid);
			if (it == _fmsById.end())
				throw type_error_uninitialized("Types::getManager() | manager not registered");
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
			catch (type_error)
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

		template<typename TFeature>
		inline bool has(::craft::instance<> const& obj)
		{
			return impl<TFeature, TFeature::craft_c_featureKind>::has(this, obj);
		}

		template<typename TFeature,
			typename std::enable_if< std::is_base_of<IIndexedProviderManager, typename TFeature::T_Manager>::value >::type* = nullptr>
		inline TFeature* getIndex(std::string name)
		{
			auto manager = getManager<TFeature>();
			auto tid = manager->index(name);
			if (tid == types::None) return nullptr;
			return (TFeature*)manager->getProvider(tid);
		}
	};

	inline Types& system()
	{
		return ::craft::types::Types::instance();
	}

	namespace _details
	{
		CRAFT_TYPES_EXPORTED void _register_type_init(TypeId id, _fn_register_type_init _init);
	}

	template<typename TFeature>
	inline TFeature* TypeId::getFeature() const
	{
		return system().get<TFeature>(*this);
	}

	template<typename TFeature>
	inline bool TypeId::hasFeature() const
	{
		return system().has<TFeature>(*this);
	}

	inline IFeatureManager* FeatureId::getManager() const
	{
		return system().getManager(*this);
	}
}}
