#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** prelude
	******************************************************************************/

	struct CppStaticDefine;

	// TODO make this a dynamic mapping, to allow user extensions
	enum class CppStaticDefineKind
	{
		Unknown,
		Abstract,
		Struct,
		Function,
		Dispatcher,
	};

	namespace details
	{
		template<typename T> class DefineHelper;

		typedef void(*CppStaticDefineRunner)(DefineHelper<void> _);
	}

	struct CppStaticDefine final
	{
	public:
		Graph::Node* node;

		details::CppStaticDefineRunner initer;
		CppStaticDefineKind kind;

		void* repr;

		// TODO: ifdef debug (or trace?) add line numbers and file names

		inline CppStaticDefine(CppStaticDefineKind kind_, void* repr_ = nullptr, details::CppStaticDefineRunner initer_ = nullptr);

		inline CppStaticDefine& operator<< (details::CppStaticDefineRunner initer_);
	};


	/******************************************************************************
	** cpp Type Id Helper
	******************************************************************************/

	/* Where as TypeId wraps a real type in the graph, type ptr wraps the C++ static define object.
	 */

	struct TypePtr final
	{
		CppStaticDefine const* desc;

		inline TypePtr() : desc(nullptr) { }
		inline TypePtr(CppStaticDefine const* const& v)
			: desc(v) { }
		inline TypePtr(TypeId const& tid)
			// TODO this is a prop
			: desc(static_cast<CppStaticDefine const*>(static_cast<Graph::Node*>(tid)->data))
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
			const TypePtr that_type = type<TType>::desc();
			return that_type.desc != nullptr && *this == that_type;
		}

		inline explicit operator CppStaticDefine const*() const { return desc; }
		inline operator TypeId() const { return asId(); }

		inline bool operator <(TypePtr const& that) const { return this->desc < that.desc; }
		inline bool operator >(TypePtr const& that) const { return this->desc > that.desc; }
		inline bool operator ==(TypePtr const& that) const { return this->desc == that.desc; }
		inline bool operator !=(TypePtr const& that) const { return this->desc != that.desc; }

		//
		// Defined in to_string.cpp
		//
		inline std::string toString() const { return asId().toString(); }
	};

	inline std::ostream & operator<<(std::ostream & s, TypePtr const & v) { s << v.toString(); return s; }

	/******************************************************************************
	** type definitions
	******************************************************************************/

	template<typename TType>
	struct type_define;

	template<>
	struct type<void>
	{
		// This must be zero width
	public:
		static constexpr CppStaticDefineKind kind = CppStaticDefineKind::Unknown;
		inline static TypePtr desc() { return nullptr; }
	};

	template <typename TType>
	struct type<TType, type_define<TType>>
	{
		static constexpr CppStaticDefineKind kind = decltype(type_define<TType>::Definition)::kind;
		inline static TypePtr desc() { return type_define<TType>::Definition; }
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
				StaticDefine,
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
		TypeStore* _store;

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
		friend inline void _dll_finish(char const*, char const*);

		friend inline void load_dll(std::string const&);

		void _init_insertEntries(_Entries* entries, size_t start);
		void _init_runEntries(_Entries* entries, size_t start);
		void _init_primeInternalEntries();

		static char const* __dll_region;

		CRAFT_TYPES_EXPORTED void _init();
		CRAFT_TYPES_EXPORTED bool _hasInited();
		CRAFT_TYPES_EXPORTED static char const* _begin(char const* name);
		CRAFT_TYPES_EXPORTED void _finish(char const* save, char const* name);
		CRAFT_TYPES_EXPORTED void _update();

	public:
		CRAFT_TYPES_EXPORTED static CppSystem& global_instance();

		//
		// Registry
		//
	private:
		void _addEntry(_Entry &&);

	public:
		inline TypeStore& types() { return *_store; }

		CRAFT_TYPES_EXPORTED void _register(CppStaticDefine const*);

		CRAFT_TYPES_EXPORTED std::string getLastLibraryName();
		CRAFT_TYPES_EXPORTED size_t getLibraryCount(std::string const& dll);
		CRAFT_TYPES_EXPORTED TypePtr getLibraryEntry(std::string const& dll, size_t index);
	};

	inline CppSystem& system()
	{
		return CppSystem::global_instance();
	}
	inline TypeStore& global_store()
	{
		return CppSystem::global_instance().types();
	}
	inline TypeStore& thread_store()
	{
		return CppSystem::global_instance().types();
	}

	inline CppStaticDefine::CppStaticDefine(CppStaticDefineKind kind_, void* repr_, details::CppStaticDefineRunner initer_)
	{
		initer = initer_;
		kind = kind_;
		repr = repr_;
		system()._register(this);
	}
	inline CppStaticDefine& CppStaticDefine::operator<< (details::CppStaticDefineRunner initer_) { initer = initer_; return *this; }

	/******************************************************************************
	** Define
	******************************************************************************/

	template <typename TType = void>
	class Define;

	template <>
	class Define<void>
	{

	};

	template <typename TType>
	class Define
	{

	};

	/******************************************************************************
	** Multimethod
	******************************************************************************/

	template <typename TDispatcher>
	class Multimethod final
	{
	private:
		CppStaticDefine __id;
		TDispatcher _dispatch;

	public:
		inline Multimethod(details::CppStaticDefineRunner init)
			: __id(CppStaticDefineKind::Dispatcher, this, init)
		{
			// TODO? throw exception if not static time
			// TODO? use a friend to initalize this thing?
		}

		inline operator Graph::Node*() const { return __id.node; }

	public:
		template <typename ...TArgs>
		inline typename TDispatcher::InvokeResult operator() (TArgs &&... args) const
		{
			// This may return an arbitrary invoke structure that forwards types and arguments
			return _dispatch.invoke( std::move(typename TDispatcher::cppArgumentsToInvoke(std::forward<TArgs>(args)...)) );
		}

		template <typename TInvoke>
		inline typename TDispatcher::InvokeResult invoke (TInvoke && invoke) const
		{
			typename TDispatcher::Dispatch d;

			TDispatcher::invokeIntoDispatch(invoke, d);

			auto res = _dispatch.dispatchWithRecord(d);
			auto callable = std::get<0>(res);
			if (callable == nullptr) throw stdext::exception("bad dispatch");
			auto dispatchRecord = std::get<1>(res);

			return TDispatcher::invoke(callable, dispatchRecord, std::move(invoke));
		}

	public:
		template<typename T>
		inline void add(T fn)
		{
			// TODO: add to the graph
			auto res = TDispatcher::cppFunctionToRecordAndFunction(fn);

			_dispatch.add(std::get<0>(res), std::get<1>(res));
		}
	};

}