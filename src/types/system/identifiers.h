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
			const TypeId that_type = types::cpptype<TType>::typeDesc();
			return that_type.id != 0 && *this == that_type;
		}

		inline bool isType(TypeId that_type)
		{
			return that_type.id != 0 && *this == that_type.id;
		}

		//
		// Defined in cpp_interface.h
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

	class Identifiers final
	{
	public:
		struct Record
		{
			void* ptr;
			TypeId ptr_type;
			TypeId id;
		};

		struct Marker
		{
			void* ptr;
			TypeId ptr_type;

			std::string name;
		};

		typedef size_t MarkerId;

	private:
		struct _Marker
		{
			Marker public_;
			size_t start;
			size_t end;
		};

		struct _Data
		{
			std::atomic<int> refcount;
			std::recursive_mutex operation;

			std::vector<Record> types; // TODO, use colony for stable pointers
			std::map<void*, TypeId> types_byPtr;

			std::vector<_Marker> markers;
			std::map<std::string, size_t> markers_byName;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	private:
		static Identifiers __global_instance;
		static thread_local Identifiers __threadlocal_instance;

		CRAFT_TYPES_EXPORTED Identifiers(Identifiers const&);
		CRAFT_TYPES_EXPORTED ~Identifiers();

	public:
		CRAFT_TYPES_EXPORTED Identifiers(bool singleton = false);

		CRAFT_TYPES_EXPORTED static Identifiers& global_instance();
		CRAFT_TYPES_EXPORTED static Identifiers& threadlocal_instance();

	//
	// Registry
	//
	public:
		CRAFT_TYPES_EXPORTED size_t count() const;

		CRAFT_TYPES_EXPORTED TypeId add(void* const& ptr, TypeId const& ptr_type);

		CRAFT_TYPES_EXPORTED Record const& get(TypeId const& id) const;
		CRAFT_TYPES_EXPORTED TypeId id(void* const& id) const;

		CRAFT_TYPES_EXPORTED void import(Identifiers const&);

		CRAFT_TYPES_EXPORTED MarkerId getMarker(std::string const&) const;
		CRAFT_TYPES_EXPORTED MarkerId startMarker(Marker const& marker);
		CRAFT_TYPES_EXPORTED void endMarker(MarkerId markerId);
		CRAFT_TYPES_EXPORTED size_t countOfMarker(MarkerId markerId) const;
	};

	inline Identifiers& identifiers()
	{
		return Identifiers::global_instance();
	}
}}
