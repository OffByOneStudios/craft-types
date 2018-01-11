#pragma once
#include "../common.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** Type Id Helper
	******************************************************************************/

	struct TypeId final
		: stdext::IdValue<TypeId, size_t>
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
}}
