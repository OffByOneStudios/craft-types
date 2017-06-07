#pragma once
#include "common.h"

namespace craft {
namespace types
{
	class type_error : public stdext::exception
	{
	public:
		explicit type_error(const std::string& msg)
			: stdext::exception(msg)
		{
		}
	};
}}
