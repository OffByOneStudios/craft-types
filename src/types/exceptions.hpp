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

	// Type identifier errors
	class type_identifier_error : public type_error
	{
	public:
		using type_error::type_error;
	};

	class type_not_found_by_identifer_error : public type_identifier_error
	{
	public:
		using type_identifier_error::type_identifier_error;
	};

	class type_identifer_not_found_error : public type_identifier_error
	{
	public:
		using type_identifier_error::type_identifier_error;
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

	// Only errors in runtime execution will throw this (e.g. C++ glue code, the system runtime not otherwise covered, and the booted system)
	class type_runtime_error : public type_error
	{
	public:
		using type_error::type_error;
	};
}}
