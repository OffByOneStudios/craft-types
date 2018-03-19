#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** prelude
	******************************************************************************/

	class Object;
	class Feature;
	class IFeatureManager;
	class Provider;
	class Aspect;

	namespace cpp
	{
		struct type_desc;
		template<typename T> class TypeDefineHelper;

		enum class CppTypeKindEnum
		{
			None,
			Object,
			RawType,
			LegacyAspect,
			LegacyProvider
		};

		bool constexpr kindIsMacroControlled(CppTypeKindEnum kind)
		{
			return kind == CppTypeKindEnum::Object
				|| kind == CppTypeKindEnum::LegacyAspect
				|| kind == CppTypeKindEnum::LegacyProvider;
		}

		typedef void(*_fn_register_type_init)(TypeDefineHelper<void> _);

		struct type_desc final
		{
		public:
			_fn_register_type_init initer;
			CppTypeKindEnum kind;
			void* repr;

			inline type_desc(CppTypeKindEnum kind_, _fn_register_type_init init_);
		};
	}

	/******************************************************************************
	** cpp Type Id Helper
	******************************************************************************/

	namespace cpp
	{
		struct TypePtr final
		{
			type_desc const* desc;

			inline TypePtr() : desc(nullptr) { }
			inline TypePtr(type_desc const* const& v) : desc(v) { }
			inline TypePtr(TypeId const& tid) : desc(static_cast<type_desc const*>(identifiers().get(tid).ptr))
			{
				// TODO assert cpp type
				//assert(identifiers().get(tid).ptr_type);
			}

			inline TypeId asId() const
			{
				return identifiers().id((void*)desc);
			}

			template<typename TType>
			inline bool isType()
			{
				const TypePtr that_type = types::type<TType>::typeDesc();
				return that_type.desc != nullptr && *this == that_type;
			}

			inline explicit operator type_desc const*() const { return desc; }
			inline operator TypeId() const { return asId(); }

			inline bool operator <(TypePtr const& that) const { return this->desc < that.desc; }
			inline bool operator >(TypePtr const& that) const { return this->desc > that.desc; }
			inline bool operator ==(TypePtr const& that) const { return this->desc == that.desc; }
			inline bool operator !=(TypePtr const& that) const { return this->desc != that.desc; }

			//
			// Defined in Types.h
			//
			template<typename TFeature> inline bool hasFeature() const { return asId().hasFeature<TFeature>(); }
			template<typename TFeature> inline TFeature* getFeature() const { return asId().getFeature<TFeature>(); }

			//
			// Defined in to_string.cpp
			//
			inline std::string toString(bool verbose = true) const { return asId().toString(); }
		};

		const static TypePtr None = nullptr;

		inline std::ostream & operator<<(std::ostream & s, TypePtr const & v) { s << v.toString(); return s; }
	}

	/******************************************************************************
	** type definitions
	******************************************************************************/

	template <typename T, typename = void> struct cpptype;
	template<>
	struct cpptype<void>
	{
		// This must be zero width
	public:
		static constexpr bool isObject = false;
		static constexpr bool isRawType = false;
		static constexpr bool isLegacyFeature = false;

		static constexpr cpp::CppTypeKindEnum kind = cpp::CppTypeKindEnum::None;
		inline static cpp::TypePtr typeDesc() { return nullptr; }
	};

	template <typename TType>
	struct cpptype<TType,
		typename std::enable_if<cpp::kindIsMacroControlled(TType::craft_c_typeKind)>::type
	>
		: public cpptype<void>
	{
		static constexpr bool isObject = TType::craft_c_typeKind == cpp::CppTypeKindEnum::Object;
		static constexpr bool isRawType = TType::craft_c_typeKind == cpp::CppTypeKindEnum::RawType;
		static constexpr bool isLegacyFeature = TType::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyAspect || TType::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyProvider;

		static constexpr cpp::CppTypeKindEnum kind = TType::craft_c_typeKind;
		inline static cpp::TypePtr typeDesc() { return TType::craft_s_typeDesc(); }
	};

	/******************************************************************************
	** Object
	******************************************************************************/

	/* Virtual base class.
	
	*/
	class Object abstract
	{
	public:
		CRAFT_TYPES_EXPORTED Object();
		CRAFT_TYPES_EXPORTED void craft_setupInstance();
		CRAFT_TYPES_EXPORTED virtual ~Object();

		virtual void* craft_instancePointer() const = 0;
		virtual cpp::TypePtr craft_typeDesc() const = 0;

		InstanceHeader* header;
	};

	/******************************************************************************
	** IFeatureManager
	******************************************************************************/

	class IFeatureManager abstract
	{
	public:
		virtual ~IFeatureManager() = default;

		virtual cpp::CppTypeKindEnum featureKind() const = 0;
		virtual std::string featureName() const = 0;
		virtual cpp::TypePtr featureDesc() const = 0;

		virtual void purgeType(cpp::TypePtr tptr) noexcept = 0;
		virtual std::vector<cpp::TypePtr> supportedTypes() const = 0;
	};

	/******************************************************************************
	** Feature
	******************************************************************************/

	/* An *Interface* in the type system

	*/
	class Feature abstract
	{
	public:
		virtual ~Feature() = default;

		virtual cpp::TypePtr craft_typeDesc() const = 0;

		inline IFeatureManager* craft_featureManager() const
		{
			return static_cast<IFeatureManager*>(craft_typeDesc().desc->repr);
		}
	};

	/******************************************************************************
	** Provider
	******************************************************************************/

	/* Interface for types

	Analogous to a collection of static methods
	*/
	class Provider abstract
		: public Feature
	{
	public:
		virtual TypeId craft_featuredTypeId() const = 0;
	};

	/******************************************************************************
	** IProviderManager
	******************************************************************************/

	class IProviderManager abstract
		: public IFeatureManager
	{
	public:
		virtual ~IProviderManager() = default;

		static const cpp::CppTypeKindEnum craft_c_managedTypeKind = cpp::CppTypeKindEnum::LegacyProvider;

		//
		// IFeatureManager
		//
		inline virtual cpp::CppTypeKindEnum featureKind() const override { return craft_c_managedTypeKind; }

		//
		// IProviderManager
		//
		virtual bool hasProvider(cpp::TypePtr tid) = 0;
		virtual Provider* getProvider(cpp::TypePtr tid) = 0;
		virtual void releaseProvider(Provider* provider) = 0;
	};

	/******************************************************************************
	** IIndexedProviderManager
	******************************************************************************/

	class IIndexedProviderManager abstract
	{
	public:
		virtual ~IIndexedProviderManager() = default;

		//
		// IIndexedProviderManager
		//
		virtual cpp::TypePtr index(std::string const&) const = 0;
	};

	/******************************************************************************
	** ITaggedProviderManager
	******************************************************************************/

	class ITaggedProviderManager abstract
	{
	public:
		virtual ~ITaggedProviderManager() = default;

		//
		// ITaggedProviderManager
		//
		virtual std::set<cpp::TypePtr> filter(std::string const&) const = 0;
	};

	/******************************************************************************
	** Aspect
	******************************************************************************/

	/* Per Instance interfaces

	Aspects require an instaniated object
	*/
	class Aspect abstract
		: public Feature
	{
	public:
		//
		// Aspect
		//
		virtual instance<> craft_featuredInstance() = 0;
	};

	/******************************************************************************
	** IAspectManager
	******************************************************************************/

	class IAspectManager abstract
		: public IFeatureManager
	{
	public:
		virtual ~IAspectManager() = default;

		static const cpp::CppTypeKindEnum craft_c_managedTypeKind = cpp::CppTypeKindEnum::LegacyAspect;

		//
		// IFeatureManager
		//
		inline virtual cpp::CppTypeKindEnum featureKind() const override { return craft_c_managedTypeKind; }

		//
		// IAspectManager
		//
		virtual bool hasAspect(cpp::TypePtr tid, void* instance) = 0;
		virtual Aspect* getAspect(cpp::TypePtr tid, void* instance) = 0;
		virtual void deleteAspect(Aspect* aspect) = 0;
	};

	/******************************************************************************
	** IAspectFactory
	******************************************************************************/

	template <typename TAspect>
	class IAspectFactory abstract
	{
	public:
		virtual ~IAspectFactory() = default;

		virtual bool isMultiUseSafe() const = 0;

		virtual TAspect* build(IAspectManager* man, void* inst) = 0;
		virtual void destroy(TAspect* aspect) = 0;
	};

	/******************************************************************************
	** CastingAspectFactory
	******************************************************************************/

	template <typename TAspect, typename TObject>
	class CastingAspectFactory
		: public IAspectFactory<TAspect>
	{
		static_assert(std::is_base_of<TAspect, TObject>::value,
			"Can only use CastingAspectFactory if `class TObject : public TAspect ...`");
	public:
		inline virtual bool isMultiUseSafe() const override { return true; }

		inline virtual TAspect* build(IAspectManager* man, void* inst) override { return (TAspect*)(TObject*)(inst); }
		inline virtual void destroy(TAspect* aspect) override { }
	};

	/******************************************************************************
	** ForwardingAspectFactory
	******************************************************************************/

	template <typename TAspect, typename TObject, typename TForwarded>
	class ForwardingAspectFactory
		: public IAspectFactory<TAspect>
	{
		typedef instance<TForwarded> TObject::* TObjectForwardingMember;

		TObjectForwardingMember _memPtr;
	public:
		inline ForwardingAspectFactory(TObjectForwardingMember mem_ptr) : _memPtr(mem_ptr) { }

		inline virtual bool isMultiUseSafe() const override { return true; }

		inline virtual TAspect* build(IAspectManager* man, void* inst) override { return (((TObject*)(inst))->*_memPtr).template getFeature<TAspect>(); }
		inline virtual void destroy(TAspect* aspect) override { }
	};

	/******************************************************************************
	** CppSystem
	******************************************************************************/

	class CppSystem final
	{
	private:

		Identifiers* _identifiers;

		TypeId _type_cpp;

		std::recursive_mutex operation;

		// 
		// Lifecycle
		//
	private:
		static CppSystem __global_instance;

		CRAFT_TYPES_EXPORTED CppSystem();
		CRAFT_TYPES_EXPORTED ~CppSystem();

		static void __s_init();

	public:
		CRAFT_TYPES_EXPORTED static CppSystem& global_instance();

		//
		// Registry
		//
	public:
		CRAFT_TYPES_EXPORTED Identifiers const& identifiers() const;
		CRAFT_TYPES_EXPORTED void init();

		CRAFT_TYPES_EXPORTED void _registerType(cpp::TypePtr);

	public:

		//
		// Managers
		//

		static inline IFeatureManager* getManager(cpp::TypePtr const& featureDesc)
		{
			return static_cast<IFeatureManager*>(featureDesc.desc->repr);
		}

		template<typename TFeature>
		static inline typename TFeature::TManager* getManager()
		{
			return static_cast<typename TFeature::TManager*>(getManager(TFeature::craft_s_typeDesc()));
		}

		template<typename TFeature>
		static inline typename TFeature::TManager* ensureManager()
		{
			cpp::TypePtr feature_desc = TFeature::craft_s_typeDesc();
			auto m = getManager(feature_desc);

			if (m == nullptr)
				const_cast<cpp::type_desc*>(feature_desc.desc)->repr = static_cast<IFeatureManager*>(new TFeature::TManager());

			return static_cast<typename TFeature::TManager*>(getManager(feature_desc));
		}

		//
		// typeGetFeature
		//

		static inline Feature* typeGetFeature(TypeId type, TypeId feature)
		{
			return nullptr; // TODO
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppTypeKindEnum::LegacyAspect >::type* = nullptr>
			static inline TFeature* typeGetFeature(instance<> const& inst)
		{
			return static_cast<TFeature*>(static_cast<IAspectManager*>(getManager<TFeature>())->getAspect(inst.typeId(), inst.get()));
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			static inline TFeature* typeGetFeature(instance<> const& inst)
		{
			return static_cast<TFeature*>(static_cast<IProviderManager*>(getManager<TFeature>())->getProvider(inst.typeId()));
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			static inline TFeature* typeGetFeature(cpp::TypePtr const& type)
		{
			return static_cast<TFeature*>(static_cast<IProviderManager*>(getManager<TFeature>())->getProvider(type));
		}

		template<typename TFeature,
			typename std::enable_if< !cpptype<TFeature>::isLegacyFeature >::type* = nullptr>
			static inline TFeature* typeGetFeature(cpp::TypePtr const& type)
		{
			return typeGetFeature(type, cpptype<TFeature>::typeDesc());
		}

		template<typename TType, typename TFeature>
		static inline TFeature* typeGetFeature()
		{
			if (std::is_base_of<TFeature, TType>::value)
				return true;
			else
				return typeGetFeature<TFeature>(cpptype<TType>::typeDesc());
		}

		//
		// typeHasFeature
		//

		static inline bool typeHasFeature(TypeId type, TypeId feature)
		{
			return false; // TODO
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppTypeKindEnum::LegacyAspect >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			return static_cast<IAspectManager*>(getManager<TFeature>())->hasAspect(type, nullptr);
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppTypeKindEnum::LegacyProvider >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			return static_cast<IProviderManager*>(getManager<TFeature>())->hasProvider(type);
		}

		template<typename TFeature,
			typename std::enable_if< !cpptype<TFeature>::isLegacyFeature >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			return typeHasFeature(type, cpptype<TFeature>::typeDesc());
		}

		template<typename TType, typename TFeature>
		static inline bool typeHasFeature()
		{
			if (std::is_base_of<TFeature, TType>::value)
				return true;
			else
				return typeHasFeature<TFeature>(cpptype<TType>::typeDesc());
		}
	};

	inline CppSystem& system()
	{
		return CppSystem::global_instance();
	}

	inline cpp::type_desc::type_desc(CppTypeKindEnum kind_, _fn_register_type_init init_)
	{
		initer = init_;
		kind = kind_;
		system()._registerType(this);
	}

	template<typename TFeature>
	inline bool TypeId::hasFeature() const
	{
		return system().typeHasFeature<TFeature>(*this);
	}
	template<typename TFeature>
	inline TFeature* TypeId::getFeature() const
	{
		return system().typeGetFeature<TFeature>(*this);
	}

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

			inline virtual TypeId craft_featuredTypeId() const override { return cpptype<TType>::typeDesc(); };
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
		inline virtual instance<> craft_featuredInstance() override { return instance<void>(header); }


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

			inline virtual instance<> craft_featuredInstance() override { return _instance; };
		};
	};

	using cpp::CppTypeKindEnum;
}}

#define CRAFT_DEFINE(x) \
	::craft::types::cpp::type_desc x::__td(x::craft_c_typeKind, (::craft::types::cpp::_fn_register_type_init)&x::__craft_s_types_init); \
	void x::__craft_s_types_init(::craft::types::cpp::TypeDefineHelper<x> _)

#define CRAFT_FORWARD_DECLARE(x, kind) \
namespace craft { namespace types { \
	template <> struct cpptype< x > \
		: public cpptype<void> \
	{ \
		static constexpr bool isObject = kind == cpp::CppTypeKindEnum::Object; \
		static constexpr bool isRawType = kind == cpp::CppTypeKindEnum::RawType; \
		static constexpr bool isLegacyFeature = kind == cpp::CppTypeKindEnum::LegacyAspect || TType::craft_c_typeKind == cpp::CppTypeKindEnum::LegacyProvider; \
		inline static constexpr cpp::CppTypeKindEnum kind() { return kind; } \
		inline static cpp::TypePtr typeDesc() { return x::craft_s_typeDesc(); } \
	}; \
}} \

#define CRAFT_OBJECT_DECLARE(x) \
    static ::craft::types::cpp::type_desc __td; \
private: \
    static void __craft_s_types_init(::craft::types::cpp::TypeDefineHelper<x> _); \
public: \
	static const ::craft::types::cpp::CppTypeKindEnum craft_c_typeKind = ::craft::types::cpp::CppTypeKindEnum::Object; \
    static inline ::craft::types::cpp::TypePtr craft_s_typeDesc() { return &x::__td; } \
	static inline ::std::string craft_s_typeName() { return #x; } \
protected: \
    inline virtual ::craft::types::cpp::TypePtr craft_typeDesc() const override { return craft_s_typeDesc(); } \
	inline virtual void* craft_instancePointer() const override { return reinterpret_cast<void*>(static_cast<x*>(const_cast<x*>(this))); } \
public: \
    inline ::craft::types::instance<x> craft_instance() const { return ::craft::types::instance<x>(static_cast<::craft::types::Object*>(const_cast<x*>(this))->header); } \
private:


#define CRAFT_OBJECT_ABSTRACT_DECLARE(x) \
private: \
    template<typename T> friend class ::craft::types::_details::TypeDefineHelper; \
	template<typename T> \
    inline static void __craft_s_types_init(::craft::types::_details::TypeDefineHelper<T> _)


#define CRAFT_LEGACY_FEATURE_DECLARE(x, name, manager) \
    static ::craft::types::cpp::type_desc __td; \
public: \
	typedef manager < x > TManager; \
private: \
	friend TManager; \
    static void __craft_s_types_init(::craft::types::cpp::TypeDefineHelper<x> _); \
public: \
	static const ::craft::types::cpp::CppTypeKindEnum craft_c_typeKind = TManager::craft_c_managedTypeKind; \
    static inline ::craft::types::cpp::TypePtr craft_s_typeDesc() { return &x::__td; } \
	static inline ::std::string craft_s_typeName() { return #x; } \
	static inline ::std::string craft_s_featureName() { return name; } \
public: \
	static inline TManager* craft_s_featureManager() \
	{ return ::craft::types::CppSystem::getManager<x>(); } \
	virtual ::craft::types::cpp::TypePtr craft_typeDesc() const override \
	{ return craft_s_typeDesc(); } \
private:


#define CRAFT_TYPE_DECLARE(_dll, x) \
namespace craft { namespace types { \
	template <> struct cpptype< x > \
		: public cpptype<void> \
	{ \
	private: \
		_dll static ::craft::types::cpp::type_desc __td; \
		static void __craft_s_types_init(::craft::types::cpp::TypeDefineHelper<x> _); \
	public: \
		static constexpr bool isRawType = true; \
		static constexpr cpp::CppTypeKindEnum kind = cpp::CppTypeKindEnum::RawType; \
		inline static cpp::TypePtr typeDesc() { return &__td; } \
	}; \
}}

#define CRAFT_TYPE_DEFINE(x) \
	::craft::types::cpp::type_desc craft::types::cpptype<x>::__td(::craft::types::cpptype<x>::kind, (::craft::types::cpp::_fn_register_type_init)&::craft::types::cpptype<x>::__craft_s_types_init); \
	void ::craft::types::cpptype<x>::__craft_s_types_init(::craft::types::cpp::TypeDefineHelper<x> _)