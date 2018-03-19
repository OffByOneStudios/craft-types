#pragma once
#include "common.h"

// Forward Declarations
namespace craft {
namespace types
{


	//
	// Detail Formward Declarations
	//

	namespace _details
	{
		const size_t _offset_table_size = 1024;
		extern unsigned char* _offset_table;

		template <typename TClass, typename TValue>
		inline size_t t_offset(TValue TClass::* TMember)
		{
			return ((size_t)(&(reinterpret_cast<const TClass*>((unsigned char const*)_details::_offset_table)->*TMember)))
				- reinterpret_cast<size_t>((unsigned char const*)_details::_offset_table);
		}

	}
}}
