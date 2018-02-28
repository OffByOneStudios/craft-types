#pragma once
#include "../common.h"
#include "graph.h"

namespace craft {
namespace types {

	class TypeGraph sealed
	{
	private:
		struct _Data
		{
			std::atomic<int> refcount;

			std::vector<SType*> types;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	private:
		static TypeGraph __global_instance;
		static thread_local TypeGraph __threadlocal_instance;

		CRAFT_TYPES_EXPORTED TypeGraph();
		CRAFT_TYPES_EXPORTED TypeGraph(TypeGraph const&);
		CRAFT_TYPES_EXPORTED ~TypeGraph();

	public:
		CRAFT_TYPES_EXPORTED static TypeGraph& global_instance();
		CRAFT_TYPES_EXPORTED static TypeGraph& threadlocal_instance();

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

		CRAFT_TYPES_EXPORTED void import(TypeGraph const&);
	};

}}
