#pragma once
#include "common.h"

namespace craft {
namespace types
{
	class type_error : public stdext::exception
	{
	public:
		using stdext::exception::exception;
	};

	class type_error_uninitialized : public type_error
	{
	public:
		using type_error::type_error;
	};
}}
