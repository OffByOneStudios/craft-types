#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	class TypeGraphSystem
	{
	private:
		std::vector<SType*> _types;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED TypeGraphSystem();
		CRAFT_TYPES_EXPORTED TypeGraphSystem(TypeGraphSystem const&);
		CRAFT_TYPES_EXPORTED ~TypeGraphSystem();

	//
	// Locking
	//
	public:
		std::recursive_mutex operation;

	//
	// Registry
	//
	public:
		CRAFT_TYPES_EXPORTED TypeId anyType() const;
		CRAFT_TYPES_EXPORTED size_t typeCount() const;

		CRAFT_TYPES_EXPORTED TypeId add(SType* type);
		CRAFT_TYPES_EXPORTED SType const* get(TypeId const& id) const;
	};

}}
