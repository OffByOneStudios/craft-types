#pragma once
#include "syn/syn.h"

namespace syn
{
	/******************************************************************************
	** TypeId
	******************************************************************************/

	struct TypeId final
	{
	private:
		Graph::Node* _node;

	public:
		inline TypeId() : _node(nullptr) {}
		inline TypeId(details::TypeIdForwardDeclare t) : _node(reinterpret_cast<Graph::Node*>(t.node)) { }
		inline TypeId(Graph::Node* n) : _node(n) { }
		inline TypeId(uintptr_t v) : _node(reinterpret_cast<Graph::Node*>(v)) {}

		inline operator Graph::Node*() const { return reinterpret_cast<Graph::Node*>(_node); }
		inline operator details::TypeIdForwardDeclare() const { return { _node }; }

		inline bool operator <(TypeId const& that) const { return this->_node < that._node; }
		inline bool operator >(TypeId const& that) const { return this->_node > that._node; }
		inline bool operator ==(TypeId const& that) const { return this->_node == that._node; }
		inline bool operator !=(TypeId const& that) const { return this->_node != that._node; }

		// Helpers defined here
		inline bool isExactType(TypeId that_type)
		{
			return that_type._node != nullptr && this->_node == that_type._node;
		}

		template<typename TType>
		inline bool isExactType()
		{
			const TypeId that_type = syn::type<TType>::graphNode();
			return isExactType(that_type);
		}

		//
		// Defined in to_string.cpp
		//
		CRAFT_TYPES_EXPORTED std::string toString() const;
	};

	const static TypeId None = nullptr;

	inline std::ostream & operator<<(std::ostream & s, TypeId const & v) { s << v.toString(); return s; }
}
