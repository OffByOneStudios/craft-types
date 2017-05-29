#pragma once
#include "common.h"

namespace craft
{
	class type_error : public stdext::exception
	{
	public:
		explicit type_error(const std::string& msg)
			: stdext::exception(msg)
		{
		}
	};
}
