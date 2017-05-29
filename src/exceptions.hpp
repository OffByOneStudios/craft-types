#pragma once
#include "common.h"

namespace craft
{
	class type_error : public craft_error
	{
	public:
		explicit type_error(const std::string& msg)
			: craft_error(msg)
		{
		}
	};
}
