#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	/******************************************************************************
	** GenericInvoke
	******************************************************************************/

	/* Helper structure for containing a generic call from the bootstrap implementation
	*/

	struct GenericInvoke
	{
		std::vector<instance<>> args;

		inline GenericInvoke(size_t presize)
		{
			args.reserve(presize);
		}
		inline GenericInvoke(instance<>* argv, size_t argc)
		{
			args.insert(args.end(), argv, argv + argc);
		}
		inline GenericInvoke(std::initializer_list<instance<>> l)
		{
			args.insert(args.end(), l.begin(), l.end());
		}
	};
}}