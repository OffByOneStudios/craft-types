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

	class type_graph_error : public type_error
	{
	public:
		using type_error::type_error;
	};

	class type_graph_not_implemented_error : public type_graph_error
	{
	public:
		using type_graph_error::type_graph_error;
	};
}}
