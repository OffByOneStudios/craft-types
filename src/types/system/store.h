#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	// A helper struct, just cast it to a TypeId.
	struct _TypeId_FwdDec
	{
		void* _node;

		inline _TypeId_FwdDec(void* _node)
			: _node(_node)
		{ }

		inline bool operator ==(_TypeId_FwdDec const& that) const { return this->_node == that._node; }
	};

	// This graph will not support import / partial usage. You will have to reimport all of it.
	// Because this graph is not meant to support deletion (replacement only) types have an incrementing index

	/******************************************************************************
	** TypeGraphTypeExtractor
	******************************************************************************/

	struct TypeGraphTypeExtractor
	{
	public:
		using Id = _TypeId_FwdDec;

		template <typename T>
		inline static Id extract();

		template <typename T>
		inline static void* convert(T const& v);
	};

	/******************************************************************************
	** TypeGraph
	******************************************************************************/

	using TypeGraph =
		craft::Graph<
			craft::GraphTyped<
				craft::GraphCore<void*>,
				TypeGraphTypeExtractor
			>
		>;

	/******************************************************************************
	** TypeStore
	******************************************************************************/

	class TypeStore final
		: public TypeGraph
	{
	private:

		std::set<char const*> _strs;

		// 
		// Lifecycle
		//
	private:
		TypeStore(TypeStore const&) = delete;

	public:
		CRAFT_TYPES_EXPORTED TypeStore();
		CRAFT_TYPES_EXPORTED TypeStore(std::vector<TypeStore*>);
		CRAFT_TYPES_EXPORTED ~TypeStore();
		
		CRAFT_TYPES_EXPORTED std::string dumpNode(Node const* n);

		CRAFT_TYPES_EXPORTED char const* malloc_cstr(char const* s);
		CRAFT_TYPES_EXPORTED char const* malloc_cstr(std::string const& s);
	};

	/******************************************************************************
	** Type Id Helper
	******************************************************************************/

	struct TypeId final
	{
	private:
		TypeGraph::Node* _node;

	public:
		inline TypeId() : _node(nullptr) {}
		inline TypeId(_TypeId_FwdDec t) : _node(reinterpret_cast<TypeGraph::Node*>(t._node)) { }
		inline TypeId(TypeGraph::Node* n) : _node(n) { }
		inline TypeId(uintptr_t v) : _node(reinterpret_cast<TypeGraph::Node*>(v)) {}

		inline operator TypeGraph::Node*() const { return reinterpret_cast<TypeGraph::Node*>(_node); }
		inline operator _TypeId_FwdDec() const { return { _node }; }
		// From IdValue, depreciate this:
		inline explicit operator uintptr_t() const { return reinterpret_cast<uintptr_t>(_node); }

		inline bool operator <(TypeId const& that) const { return this->_node < that._node; }
		inline bool operator >(TypeId const& that) const { return this->_node > that._node; }
		inline bool operator ==(TypeId const& that) const { return this->_node == that._node; }
		inline bool operator !=(TypeId const& that) const { return this->_node != that._node; }

		// Helpers defined here
		template<typename TType>
		inline bool isType()
		{
			const TypeId that_type = types::cpptype<TType>::typeDesc();
			return that_type._node != nullptr && this->_node == that_type._node;
		}

		inline bool isType(TypeId that_type)
		{
			return that_type._node != nullptr && this->_node == that_type._node;
		}

		std::string identifier() const;

		//
		// Defined in cpp_interface.h
		//
		template<typename TFeature> inline bool hasFeature() const;
		template<typename TFeature> inline TFeature* getFeature() const;

		//
		// Defined in to_string.cpp
		//
		CRAFT_TYPES_EXPORTED std::string toString() const;
	};

	const static TypeId None = nullptr;

	inline std::ostream & operator<<(std::ostream & s, TypeId const & v) { s << v.toString(); return s; }

	/******************************************************************************
	** inline defines
	******************************************************************************/

	template<typename T>
	inline TypeGraphTypeExtractor::Id TypeGraphTypeExtractor::extract()
	{
		return _TypeId_FwdDec((void*)craft::types::cpptype<T>::typeDesc().desc);
	}
	
	template<typename T>
	inline void* TypeGraphTypeExtractor::convert(T const& v)
	{
		if (sizeof(T) <= sizeof(void*))
			return (void*)reinterpret_cast<uintptr_t const&>(v);
		
		return (void*) new T(v);
	}

	/******************************************************************************
	** IGraphIndex
	******************************************************************************/

/*
	// This represents indexs inside the graph
	struct IGraphIndex
	{
		inline virtual ~IGraphIndex() = default;

		typedef void (*fn_update)(IGraphIndex* index, void const* value, Graph::Node* node);
		typedef Graph::Node* (*fn_lookup)(IGraphIndex* index, void const* value);

		fn_update update;
		fn_lookup lookup;
	};
*/

/*
	// This represents indexs inside the graph
	template <typename T>
	struct BasicGraphIndex
		: IGraphIndex
	{
		typedef T input_type;

		inline virtual ~BasicGraphIndex() = default;

		std::map<std::string, Graph::Node*> index;

	private:
		inline void _update(char const* v, Graph::Node* node)
		{
			index[v] = node;
		}
		inline void _update(std::string* v, Graph::Node* node)
		{
			index[*v] = node;
		}

		inline Graph::Node* _find(char const* v)
		{
			auto it = index.find(v);
			if (it != index.end())
				return it->second;
			return nullptr;
		}
		inline Graph::Node* _find(std::string* v)
		{
			auto it = index.find(*v);
			if (it != index.end())
				return it->second;
			return nullptr;
		}

		inline std::vector<Graph::Node*> _list()
		{
			std::vector<Graph::Node*> res;
			std::transform(index.begin(), index.end(), std::back_inserter(res), [](auto it) { return it.second; });
			return res;
		}

	public:
		inline static void s_update(IGraphIndex* index, void const* value, Graph::Node* node)
		{
			((BasicGraphIndex<T>*)index)->_update((T)value, node);
		}
		inline static Graph::Node* s_lookup(IGraphIndex* index, void const* value)
		{
			return ((BasicGraphIndex<T>*)index)->_find((T)value);
		}

		inline BasicGraphIndex()
		{
			update = s_update;
			lookup = s_lookup;
		}

		inline static Graph::Node* deindex(IGraphIndex* index, T value)
		{
			return ((BasicGraphIndex<T>*)index)->_find(value);
		}

		inline static std::vector<Graph::Node*> listvalues(IGraphIndex* index)
		{
			return ((BasicGraphIndex<T>*)index)->_list();
		}
	};
*/

	/******************************************************************************
	** GraphPropertyMetaIndex
	******************************************************************************/

/*
	// Internal graph printer, not meant for use as a "to string" method.
	struct GraphPropertyMetaIndex final
	{
		// is an arbitrary pointer as determined by the property meta
		typedef IGraphIndex* value_type;
	private:
		GraphPropertyMetaIndex() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "meta.index";
		static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};

	template<typename T>
	inline Graph::Node* Graph::getByIndex(typename T::index_type::input_type key)
	{
		auto metaNode = meta<T>();
		auto index = (typename T::index_type*)getFirstPropValue<GraphPropertyMetaIndex>(metaNode);
		return T::index_type::deindex(index, key);
	}

	template<typename T>
	inline std::vector<Graph::Node*> Graph::listIndex()
	{
		auto metaNode = meta<T>();
		auto index = (typename T::index_type*)getFirstPropValue<GraphPropertyMetaIndex>(metaNode);
		return T::index_type::listvalues(index);
	}
*/

	/******************************************************************************
	** GraphPropertyPrinter
	******************************************************************************/

/*
	// Internal graph printer, not meant for use as a "to string" method.
	struct GraphPropertyPrinter final
	{
		// is a   `char const*`
		typedef std::string(*value_type)(Graph::Element*);
	private:
		GraphPropertyPrinter() = delete;
	public:
		static constexpr GraphMeta::Kind craftTypes_metaKind = GraphMeta::Kind::Prop; // needed?
		static constexpr char const* craftTypes_metaProp_name = "printer";
		inline static GraphPropMeta* craftTypes_metaProp_builder(Graph::Node* metanode)
		{
			return GraphPropMeta::Singular(craftTypes_metaProp_name);
		}
	};
*/


}}
