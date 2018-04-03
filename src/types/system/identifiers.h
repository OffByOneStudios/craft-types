#pragma once
#include "../common.h"

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
			TypeId id;
			void* node;
		};

	private:

		struct _Data
		{
			std::atomic<int> refcount;
			std::recursive_mutex operation;

			plf::colony <Record> types;
			std::map<void*, TypeId> types_byPtr;
		};

		_Data* _contents;

	// 
	// Lifecycle
	//
	public:
		CRAFT_TYPES_EXPORTED Identifiers();
		CRAFT_TYPES_EXPORTED Identifiers(Identifiers const&);
		CRAFT_TYPES_EXPORTED ~Identifiers();

	//
	// Registry
	//
	public:
		CRAFT_TYPES_EXPORTED size_t count() const;

		CRAFT_TYPES_EXPORTED TypeId add(void* const& ptr, void* const& node_ptr);

		CRAFT_TYPES_EXPORTED Record const& get(TypeId const& id) const;
		CRAFT_TYPES_EXPORTED TypeId id(void* const& id) const;
	};
}}
