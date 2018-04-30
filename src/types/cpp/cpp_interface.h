#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	// For the feature redesign: https://stackoverflow.com/questions/4936987/c-class-member-variable-knowing-its-own-offset

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
		struct static_desc;
		template<typename T> class DefineHelper;

		enum class CppStaticDescKindEnum
		{
			None,
			Object,
			RawType,
			LegacyAspect,
			LegacyProvider,
			MultiMethod,
			UserInfo,
		};

		bool constexpr kindIsMacroControlled(CppStaticDescKindEnum kind)
		{
			return kind == CppStaticDescKindEnum::Object
				|| kind == CppStaticDescKindEnum::LegacyAspect
				|| kind == CppStaticDescKindEnum::LegacyProvider;
		}
		bool constexpr kindIsType(CppStaticDescKindEnum kind)
		{
			return kind == CppStaticDescKindEnum::Object
				|| kind == CppStaticDescKindEnum::RawType
				|| kind == CppStaticDescKindEnum::LegacyAspect
				|| kind == CppStaticDescKindEnum::LegacyProvider;
		}
		bool constexpr kindIsInfo(CppStaticDescKindEnum kind)
		{
			return kind == CppStaticDescKindEnum::MultiMethod
				|| kind == CppStaticDescKindEnum::UserInfo;
		}

		typedef void(*_fn_register_static_init)(DefineHelper<void> _);

		struct static_desc final
		{
		public:
			Graph::Node* node;
			_fn_register_static_init initer;
			CppStaticDescKindEnum kind;
			void* repr;

			inline static_desc(CppStaticDescKindEnum kind_, void* repr_ = nullptr, _fn_register_static_init initer_ = nullptr);

			inline static_desc& operator<< (_fn_register_static_init initer_);
		};
	}

	/******************************************************************************
	** cpp Type Id Helper
	******************************************************************************/

	namespace cpp
	{
		struct TypePtr final
		{
			static_desc const* desc;

			inline TypePtr() : desc(nullptr) { }
			inline TypePtr(static_desc const* const& v) : desc(v) { assert(desc == nullptr || kindIsType(desc->kind)); }
			inline TypePtr(TypeId const& tid) : desc(static_cast<static_desc const*>(tid.node->value))
			{
				// TODO assert cpp type
				//assert(identifiers().get(tid).ptr_type);
			}

			inline TypeId asId() const
			{
				if (desc == nullptr) return nullptr;
				return desc->node;
			}

			template<typename TType>
			inline bool isType()
			{
				const TypePtr that_type = types::cpptype<TType>::typeDesc();
				return that_type.desc != nullptr && *this == that_type;
			}

			inline explicit operator static_desc const*() const { return desc; }
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
			inline std::string toString(bool verbose = true) const { return asId().toString(verbose); }
		};

		const static TypePtr None = nullptr;

		inline std::ostream & operator<<(std::ostream & s, TypePtr const & v) { s << v.toString(); return s; }
	}

	/******************************************************************************
	** type definitions
	******************************************************************************/

	template<>
	struct cpptype<void>
	{
		// This must be zero width
	public:
		static constexpr bool isObject = false;
		static constexpr bool isRawType = false;
		static constexpr bool isLegacyFeature = false;

		static constexpr cpp::CppStaticDescKindEnum kind = cpp::CppStaticDescKindEnum::None;
		inline static cpp::TypePtr typeDesc() { return nullptr; }
	};

	template <typename TType>
	struct cpptype<TType,
		typename std::enable_if<cpp::kindIsMacroControlled(TType::craft_c_typeKind)>::type
	>
		: public cpptype<void>
	{
		static constexpr bool isObject = TType::craft_c_typeKind == cpp::CppStaticDescKindEnum::Object;
		static constexpr bool isRawType = TType::craft_c_typeKind == cpp::CppStaticDescKindEnum::RawType;
		static constexpr bool isLegacyFeature = TType::craft_c_typeKind == cpp::CppStaticDescKindEnum::LegacyAspect || TType::craft_c_typeKind == cpp::CppStaticDescKindEnum::LegacyProvider;

		static constexpr cpp::CppStaticDescKindEnum kind = TType::craft_c_typeKind;
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

		InstanceHeader* craft_header;
	};

	/******************************************************************************
	** IFeatureManager
	******************************************************************************/

	class IFeatureManager abstract
	{
	public:
		virtual ~IFeatureManager() = default;

		virtual cpp::CppStaticDescKindEnum featureKind() const = 0;
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

		static const cpp::CppStaticDescKindEnum craft_c_managedTypeKind = cpp::CppStaticDescKindEnum::LegacyProvider;

		//
		// IFeatureManager
		//
		inline virtual cpp::CppStaticDescKindEnum featureKind() const override { return craft_c_managedTypeKind; }

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

		static const cpp::CppStaticDescKindEnum craft_c_managedTypeKind = cpp::CppStaticDescKindEnum::LegacyAspect;

		//
		// IFeatureManager
		//
		inline virtual cpp::CppStaticDescKindEnum featureKind() const override { return craft_c_managedTypeKind; }

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

		struct _Entry
		{
			enum class Kind
			{
				StaticDesc,
				Marker,
				Warning
			};

			void* ptr;
			Kind kind;
		};

		struct _Entries
		{
			std::vector<_Entry> _entries;
		};

	private:
		// These first for inlined functions
		// Graph for this cpp-system (todo: invert this, graph is also a static)
		Graph* _graph;

		std::recursive_mutex operation;

		_Entries* _static_entries;

		_Entries* _current_dll_entries;
		std::map<std::string, _Entries*> _dll_entries;
		std::set<std::string> _dllsToUpdate;
		std::set<std::string> _dllsThatWereStatic;

		std::string _lastLoadedDll;

		// 
		// Lifecycle
		//
	private:
		CppSystem();
		~CppSystem();

		friend inline void boot();
		friend inline char const* _dll_begin(char const*);
		friend inline void _dll_finish(char const*);

		friend inline void load_dll(std::string const&);

		void _init_insertEntries(_Entries* entries, size_t start);
		void _init_runEntries(_Entries* entries, size_t start);

		static char const* __dll_region;

		CRAFT_TYPES_EXPORTED void _init();
		CRAFT_TYPES_EXPORTED bool _hasInited();
		CRAFT_TYPES_EXPORTED static char const* _begin(char const* name);
		CRAFT_TYPES_EXPORTED void _finish(char const* name);
		CRAFT_TYPES_EXPORTED void _update();

	public:
		CRAFT_TYPES_EXPORTED static CppSystem& global_instance();

		//
		// Registry
		//
	private:
		void _addEntry(_Entry &&);

	public:
		inline Graph& graph() { return *_graph; }

		CRAFT_TYPES_EXPORTED void _register(cpp::static_desc const*);

		CRAFT_TYPES_EXPORTED std::string getLastLibraryName();
		CRAFT_TYPES_EXPORTED size_t getLibraryCount(std::string const& dll);
		CRAFT_TYPES_EXPORTED cpp::TypePtr getLibraryEntry(std::string const& dll, size_t index);

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
				const_cast<cpp::static_desc*>(feature_desc.desc)->repr = static_cast<IFeatureManager*>(new typename TFeature::TManager());

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
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyAspect >::type* = nullptr>
			static inline TFeature* typeGetFeature(instance<> const& inst)
		{
			return static_cast<TFeature*>(static_cast<IAspectManager*>(getManager<TFeature>())->getAspect(inst.typeId(), inst.get()));
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
			static inline TFeature* typeGetFeature(instance<> const& inst)
		{
			return static_cast<TFeature*>(static_cast<IProviderManager*>(getManager<TFeature>())->getProvider(inst.typeId()));
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
			static inline TFeature* typeGetFeature(cpp::TypePtr const& type)
		{
			return static_cast<TFeature*>(static_cast<IProviderManager*>(getManager<TFeature>())->getProvider(type));
		}

		template<typename TFeature,
			typename std::enable_if< !cpptype<TFeature>::isLegacyFeature >::type* = nullptr>
			static inline TFeature* typeGetFeature(cpp::TypePtr const& type)
		{
			// Runtime answer for feature-types that we have not compile time knowledge of
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
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyAspect >::type* = nullptr>
			static inline bool typeHasFeature(instance<> const& inst)
		{
			return static_cast<IAspectManager*>(getManager<TFeature>())->hasAspect(inst.typeId(), inst.get());
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
			static inline bool typeHasFeature(instance<> const& inst)
		{
			return static_cast<IProviderManager*>(getManager<TFeature>())->hasProvider(inst.typeId());
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyAspect >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			return static_cast<IAspectManager*>(getManager<TFeature>())->hasAspect(type, nullptr);
		}

		template<typename TFeature,
			typename std::enable_if< cpptype<TFeature>::kind == cpp::CppStaticDescKindEnum::LegacyProvider >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			return static_cast<IProviderManager*>(getManager<TFeature>())->hasProvider(type);
		}

		template<typename TFeature,
			typename std::enable_if< !cpptype<TFeature>::isLegacyFeature >::type* = nullptr>
			static inline bool typeHasFeature(cpp::TypePtr const& type)
		{
			// Runtime answer for feature-types that we have not compile time knowledge of
			return typeHasFeature(type, cpptype<TFeature>::typeDesc());
		}

		template<typename TType, typename TFeature>
		static inline bool typeHasFeature()
		{
			if (std::is_base_of<TFeature, TType>::value) // Compile time check has an answer
				return true; // Compile time answer
			else
				return typeHasFeature<TFeature>(cpptype<TType>::typeDesc()); // Runtime answer
		}
	};

	inline CppSystem& system()
	{
		return CppSystem::global_instance();
	}
	inline Graph& graph()
	{
		return CppSystem::global_instance().graph();
	}

	inline cpp::static_desc::static_desc(CppStaticDescKindEnum kind_, void* repr_, _fn_register_static_init initer_)
	{
		initer = initer_;
		kind = kind_;
		repr = repr_;
		system()._register(this);
	}
	inline cpp::static_desc& cpp::static_desc::operator<< (cpp::_fn_register_static_init initer_) { initer = initer_; }

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
	** cpp::Multimethod
	******************************************************************************/

	namespace cpp
	{
		template<typename TDispatcher>
		class Multimethod final
			: public types::Multimethod<Function, TDispatcher>
		{
		private:
			static_desc __id;

		public:
			inline Multimethod(_fn_register_static_init init)
				: __id(CppStaticDescKindEnum::MultiMethod, this, init)
			{
				// TODO throw exception if not static time
			}

			inline operator types::Graph::Node*() const { return __id.node; }

		public:
			template <typename ...TArgs>
			inline typename TDispatcher::InvokeResult operator() (TArgs &&... args) const
			{
				// This may return an arbitrary invoke structure that forwards types and arguments
				return invoke( std::move(typename TDispatcher::cppArgumentsToInvoke(std::forward<TArgs>(args)...)) );
			}

			template <typename TInvoke>
			inline typename TDispatcher::InvokeResult invoke (TInvoke && invoke) const
			{
				typename TDispatcher::Dispatch d;

				typename TDispatcher::invokeIntoDispatch(invoke, d);

				auto res = this->dispatchWithRecord(d);
				auto callable = std::get<0>(res);
				if (callable == nullptr) throw stdext::exception("bad dispatch");
				auto dispatchRecord = std::get<1>(res);

				return typename TDispatcher::invoke(callable, dispatchRecord, std::move(invoke));
			}

		public:
			template<typename T>
			inline void add(T fn)
			{
				auto res = TDispatcher::cppFunctionToRecordAndFunction(fn);

				addRecord(std::get<0>(res), std::get<1>(res));
			}
		};
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
		inline virtual instance<> craft_featuredInstance() override { return instance<void>(craft_header); }


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

	using cpp::CppStaticDescKindEnum;
}}

#define _CRAFT_PP_CONCAT_0(a, b) _CRAFT_PP_CONCAT_1(a, b)
#define _CRAFT_PP_CONCAT_1(a, b) _CRAFT_PP_CONCAT_2(~, a ## b)
#define _CRAFT_PP_CONCAT_2(p, res) res

#define CRAFT_DEFINE(x) \
	::craft::types::cpp::static_desc x::__td(x::craft_c_typeKind, nullptr, (::craft::types::cpp::_fn_register_static_init)&x::__craft_s_static_init); \
	void x::__craft_s_static_init(::craft::types::cpp::DefineHelper<x> _)

#define CRAFT_TYPE_DEFINE(x) \
	::craft::types::cpp::static_desc craft::types::cpptype<x>::__td(::craft::types::cpptype<x>::kind, nullptr, (::craft::types::cpp::_fn_register_static_init)&::craft::types::cpptype<x>::__craft_s_static_init); \
	void ::craft::types::cpptype<x>::__craft_s_static_init(::craft::types::cpp::DefineHelper<x> _)

#define CRAFT_MULTIMETHOD_DEFINE(x) \
	void _CRAFT_PP_CONCAT_0(__fninit_, __LINE__) (::craft::types::cpp::DefineHelper<decltype(x)> _); \
	decltype(x) x ((::craft::types::cpp::_fn_register_static_init) (& _CRAFT_PP_CONCAT_0(__fninit_, __LINE__))); \
	void _CRAFT_PP_CONCAT_0(__fninit_, __LINE__) (::craft::types::cpp::DefineHelper<decltype(x)> _)

#define CRAFT_INFO_MORE(x) \
	void _CRAFT_PP_CONCAT_0(__fninit_, __LINE__) (::craft::types::cpp::DefineHelper<x> _); \
	::craft::types::cpp::static_desc _CRAFT_PP_CONCAT_0(__id_, __LINE__) (x::craft_c_infoKind + ::craft::types::cpp::CppInfoKindEnum::NoneMore, nullptr, (::craft::types::cpp::_fn_register_static_init) (& _CRAFT_PP_CONCAT_0(__fninit_, __LINE__))); \
	void _CRAFT_PP_CONCAT_0(__fninit_, __LINE__) (::craft::types::cpp::DefineHelper<x> _)

#define CRAFT_FORWARD_DECLARE(x, kindv) \
namespace craft { namespace types { \
	template <> struct cpptype< x > \
		: public cpptype<void> \
	{ \
		static constexpr bool isObject = kindv == cpp::CppStaticDescKindEnum::Object; \
		static constexpr bool isRawType = kindv == cpp::CppStaticDescKindEnum::RawType; \
		static constexpr bool isLegacyFeature = kindv == cpp::CppStaticDescKindEnum::LegacyAspect || kindv == cpp::CppStaticDescKindEnum::LegacyProvider; \
		static constexpr cpp::CppStaticDescKindEnum kind = kindv; \
		inline static cpp::TypePtr typeDesc(); \
	}; \
}} \

#define CRAFT_FORWARD_DECLARE_COMPLETE(x) \
inline craft::types::cpp::TypePtr craft::types::cpptype< x >::typeDesc() { return x::craft_s_typeDesc(); }

#define CRAFT_OBJECT_DECLARE(x) \
    static ::craft::types::cpp::static_desc __td; \
private: \
    static void __craft_s_static_init(::craft::types::cpp::DefineHelper<x> _); \
public: \
	static const ::craft::types::cpp::CppStaticDescKindEnum craft_c_typeKind = ::craft::types::cpp::CppStaticDescKindEnum::Object; \
    static inline ::craft::types::cpp::TypePtr craft_s_typeDesc() { return &x::__td; } \
	static inline char const* craft_s_typeName() { return #x; } \
protected: \
    inline virtual ::craft::types::cpp::TypePtr craft_typeDesc() const override { return craft_s_typeDesc(); } \
	inline virtual void* craft_instancePointer() const override { return reinterpret_cast<void*>(static_cast<x*>(const_cast<x*>(this))); } \
public: \
    inline ::craft::types::instance<x> craft_instance() const { return ::craft::types::instance<x>(static_cast<::craft::types::Object*>(const_cast<x*>(this))->craft_header); } \
private:


#define CRAFT_LEGACY_FEATURE_DECLARE(x, name, manager) \
    static ::craft::types::cpp::static_desc __td; \
public: \
	typedef manager < x > TManager; \
private: \
	friend TManager; \
    static void __craft_s_static_init(::craft::types::cpp::DefineHelper<x> _); \
public: \
	static const ::craft::types::cpp::CppStaticDescKindEnum craft_c_typeKind = TManager::craft_c_managedTypeKind; \
    static inline ::craft::types::cpp::TypePtr craft_s_typeDesc() { return &x::__td; } \
	static inline char const* craft_s_typeName() { return #x; } \
	static inline char const* craft_s_featureName() { return name; } \
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
		_dll static ::craft::types::cpp::static_desc __td; \
		static void __craft_s_static_init(::craft::types::cpp::DefineHelper<x> _); \
	public: \
		static constexpr bool isRawType = true; \
		static constexpr cpp::CppStaticDescKindEnum kind = cpp::CppStaticDescKindEnum::RawType; \
		inline static cpp::TypePtr typeDesc() { return &__td; } \
		static inline char const* typeName() { return #x; } \
	}; \
}}

#define CRAFT_MULTIMETHOD_DECLARE(x, dispatcher) \
extern ::craft::types::cpp::Multimethod< dispatcher > x;