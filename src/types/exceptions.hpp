#pragma once
#include "common.h"

namespace craft {
namespace types
{
	// All errors in this library throw this
	class type_error : public stdext::exception
	{
	public:
		using stdext::exception::exception;
	};

	// Something is unintialized, meant to be internal
	class type_error_uninitialized : public type_error
	{
	public:
		using type_error::type_error;
	};

	// Type graph errors
	class type_graph_error : public type_error
	{
	public:
		using type_error::type_error;
	};

	// Unimplement feature inside the type graph (not the types themselves).
	class type_graph_not_implemented_error : public type_graph_error
	{
	public:
		using type_graph_error::type_graph_error;
	};
}}
