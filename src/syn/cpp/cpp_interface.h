#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** prelude
	******************************************************************************/

	struct CppDefine;

	// TODO make this a dynamic mapping, to allow user extensions
	enum class CppDefineKind
	{
		Unknown,    // unknown / void, treat like information element
		Abstract,   // presents as a C++ abstract, could be interface or abstract base
		Struct,     // presents as a concrete C++ object, treat like structure type
		Dispatcher, // is an explicit Multimethod define
		Module,     // is an explicit Module define
	};

	namespace details
	{
		template<typename T> class DefineHelper;

		template<typename T = void>
		using CppDefineRunner = void(*)(DefineHelper<T>& _);
	}

	// should always be static...
	struct CppDefine
	{
	public:
		Graph::Node* node;

		details::CppDefineRunner<> initer;
		CppDefineKind kind;

		void* repr;

		// TODO: ifdef debug (or trace?) add line numbers and file names

		inline CppDefine(CppDefineKind kind_, void* repr_ = nullptr, details::CppDefineRunner<> initer_ = nullptr);

		inline CppDefine& operator<< (details::CppDefineRunner<> initer_);
	};


	/******************************************************************************
	** cpp Type Id Helper
	******************************************************************************/

	/* Where as TypeId wraps a real type in the graph, type ptr wraps the C++ static define object.
	 */

	struct TypePtr final
	{
		CppDefine const* desc;

		inline TypePtr() : desc(nullptr) { }
		inline TypePtr(CppDefine const* const& v)
			: desc(v) { }
		inline TypePtr(TypeId const& tid)
			// TODO this is a prop
			: desc(static_cast<CppDefine const*>(static_cast<Graph::Node const*>(tid)->data))
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

		inline explicit operator CppDefine const*() const { return desc; }
		inline operator TypeId() const { return asId(); }

		inline bool operator <(TypePtr const& that) const { return this->desc < that.desc; }
		inline bool operator >(TypePtr const& that) const { return this->desc > that.desc; }
		inline bool operator ==(TypePtr const& that) const { return this->desc == that.desc; }
		inline bool operator !=(TypePtr const& that) const { return this->desc != that.desc; }

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
		static constexpr CppDefineKind kind = CppDefineKind::Unknown;
		inline static TypePtr desc() { return nullptr; }
		inline static TypeId id() { return nullptr; }
		inline static Graph::Node const* graphNode() { return nullptr; }
	};

	template <typename TType>
	struct type<TType, std::void_t<type_define<TType>>>
	{
		static constexpr CppDefineKind kind = decltype(type_define<TType>::Definition)::kind;
		inline static TypePtr desc() { return &type_define<TType>::Definition; }
		inline static TypeId id() { return desc().asId(); }
		inline static Graph::Node const* graphNode() { return id(); }
	};

}

#include "CppSystem.h"

namespace syn
{
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

	inline CppDefine::CppDefine(CppDefineKind kind_, void* repr_, details::CppDefineRunner<> initer_)
	{
		initer = initer_;
		kind = kind_;
		repr = repr_;
		system()._register(this);
	}
	inline CppDefine& CppDefine::operator<< (details::CppDefineRunner<> initer_) { initer = initer_; return *this; }

	/******************************************************************************
	** Define
	******************************************************************************/

	template <typename TType = void>
	class Define;

	// should always be static...
	template <>
	class Define<void> final
		: public CppDefine
	{
	public:
		static constexpr CppDefineKind kind = CppDefineKind::Unknown;

		Define(details::CppDefineRunner<> init)
			: CppDefine(kind, this, init)
			{ }
	};

	// should always be static...
	// TODO do discrimination
	template <typename TType>
	class Define final
		: public CppDefine
	{
	public:
		static constexpr CppDefineKind kind = CppDefineKind::Struct;
		typedef TType Type;

		Define(details::CppDefineRunner<Define<TType>> init)
			: CppDefine(kind, this, reinterpret_cast<details::CppDefineRunner<>>(init))
			{ }
	};

	/******************************************************************************
	** Module
	******************************************************************************/

	class Module final
		: public CppDefine
		, public ModuleBase
	{
	public:
		static constexpr CppDefineKind kind = CppDefineKind::Module;

		inline Module(details::CppDefineRunner<Module> init)
			: CppDefine(kind, this, reinterpret_cast<details::CppDefineRunner<>>(init))
		{ }
	};

	/******************************************************************************
	** Multimethod
	******************************************************************************/

	// should always be static...
	template <typename TDispatcher>
	class Multimethod final
		: CppDefine
	{
	private:
		TDispatcher _dispatch;

	public:
		static constexpr CppDefineKind kind = CppDefineKind::Dispatcher;

		inline Multimethod(details::CppDefineRunner<Multimethod<TDispatcher>> init)
			: CppDefine(kind, this, reinterpret_cast<details::CppDefineRunner<>>(init))
		{
			// TODO? throw exception if not static time
			// TODO? use a friend to initalize this thing?
		}

		inline operator Graph::Node*() const { return this->node; }

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