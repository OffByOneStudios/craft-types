#pragma once
#include "common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type Id Helper
	******************************************************************************/

	struct TypeId final
		: stdext::IdValue<TypeId, uintptr_t>
	{
		using IdValue::IdValue;

		template<typename TType>
		inline bool isType()
		{
			const TypeId that_type = types::type<TType>::typeId();
			return that_type.id != 0 && *this == that_type;
		}

		//
		// Defined in Types.h
		//
		template<typename TFeature> inline bool hasFeature() const;
		template<typename TFeature> inline TFeature* getFeature() const;

		//
		// Defined in to_string.cpp
		//
		CRAFT_TYPES_EXPORTED std::string toString(bool verbose = true) const;
	};

	const static TypeId None = 0;

	inline std::ostream & operator<<(std::ostream & s, TypeId const & v) { s << v.toString(); return s; }

	/******************************************************************************
	** Identifiers
	******************************************************************************/

	class Identifiers sealed
	{
	public:
		struct Record
		{
			void* ptr;
			TypeId ptr_type;
			TypeId id;
		};

	private:
		struct _Data
		{
			std::atomic<int> refcount;
			std::recursive_mutex operation;

			std::vector<Record> types; // TODO, use colony for stable pointers
			std::map<void*, TypeId> types_byPtr;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	private:
		static Identifiers __global_instance;
		static thread_local Identifiers __threadlocal_instance;

		CRAFT_TYPES_EXPORTED Identifiers();
		CRAFT_TYPES_EXPORTED Identifiers(Identifiers const&);
		CRAFT_TYPES_EXPORTED ~Identifiers();

	public:
		CRAFT_TYPES_EXPORTED static Identifiers& global_instance();
		CRAFT_TYPES_EXPORTED static Identifiers& threadlocal_instance();

	//
	// Registry
	//
	public:
		CRAFT_TYPES_EXPORTED size_t count() const;

		CRAFT_TYPES_EXPORTED TypeId add(void* const& ptr, TypeId const& ptr_type);
		CRAFT_TYPES_EXPORTED void patch(TypeId const& to_patch, TypeId const& ptr_type);

		CRAFT_TYPES_EXPORTED Record const& get(TypeId const& id) const;
		CRAFT_TYPES_EXPORTED TypeId id(void* const& id) const;

		CRAFT_TYPES_EXPORTED void import(Identifiers const&);
	};

	inline Identifiers& identifiers()
	{
		return Identifiers::global_instance();
	}
}}
