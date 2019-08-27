#pragma once
#include "syn/syn.h"

/* This is the instantiation of the graph library and it's helpers.
 * 
 * Most of the interesting code lives in the graph library, the majority of this file is a song
 * and dance to get the Grph to use self referencing types.
 */

namespace syn
{
	/******************************************************************************
	** prelude
	******************************************************************************/

    namespace details
    {
        // A helper struct, so we can forward declare TypeId into the graph
		// also provides *mutable* access.
        struct TypeIdForwardDeclare
        {
            void* node;

            inline TypeIdForwardDeclare()
                : node(nullptr)
            { }

            inline TypeIdForwardDeclare(void const* node)
                : node(const_cast<void*>(node))
            { }

            inline bool operator ==(TypeIdForwardDeclare const& that) const { return this->node == that.node; }
        };
    }

	/******************************************************************************
	** GraphConfig
	******************************************************************************/

	// This graph will not support import / partial usage. You will have to reimport all of it if you want multiple type graphs.

	struct GraphConfig
		: graph::basic_core_config<void*>
	{
	public:
		using TypeId = details::TypeIdForwardDeclare;

		template <typename T>
		inline static TypeId typed_typeToValue();

		template <typename T>
		inline static void* typed_store(T const& v);
		template <typename T>
		inline static T* typed_load(void* const&);
	};

	/******************************************************************************
	** Graph
	******************************************************************************/

	using Graph =
		graph::Graph<
			graph::GraphTyped<
				graph::GraphCore<GraphConfig>
			>
		>;

	/******************************************************************************
	** GraphConfig inline defines
	******************************************************************************/

	template<typename T>
	inline GraphConfig::TypeId GraphConfig::typed_typeToValue()
	{
		return (details::TypeIdForwardDeclare) syn::type<T>::graphNode();
	}
	
	template<typename T>
	inline void* GraphConfig::typed_store(T const& t)
	{
		// Invokes copy constructor once in either path

		if constexpr (sizeof(T) <= sizeof(void*))
		{
			// Pack `t` into a void*
			void* v = nullptr;
			void** vp = &v;
			T* tp = (T*) vp; // This is very bad if not for the sizeof proof above.
			*tp = t;
			return v;
		}
		else
		{
			return (void*) new T(t);
		}
	}
	
	template<typename T>
	inline T* GraphConfig::typed_load(void* const& v)
	{
		if (v == nullptr) return nullptr;

		if constexpr (sizeof(T) <= sizeof(void*))
		{
			void** vp = const_cast<void**>(&v);
			return (T*) vp;
		}
		else
		{
			return (T*) v;
		}
	}

	/******************************************************************************
	** epilouge
	******************************************************************************/

    namespace details
    {
		inline Graph::Node* node(TypeIdForwardDeclare id)
		{
			return reinterpret_cast<Graph::Node*>(id.node);
		}
    }
}
