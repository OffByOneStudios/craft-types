#pragma once
#include "../common.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** PParse
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PParse abstract
		: public Provider
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(PParse, "types.parse", SingletonProviderManager);

	public:
		virtual instance<> parse(std::string str) const noexcept = 0;
	};

	/******************************************************************************
	** FunctionalParse
	******************************************************************************/

	template <typename T>
	class FunctionalParse
		: public Implements<PParse>::For<T>
	{
	public:
		typedef instance<T> TParseRet;

	private:
		std::function< TParseRet(std::string) > _parser;

	public:
		inline FunctionalParse(std::function< TParseRet(std::string) > const& parser)
			: _parser(parser)
		{ }

		inline virtual instance<> parse(std::string str) const noexcept override
		{
			try
			{
				return _parser(str);
			}
			catch(...)
			{
				// TODO, create and return an error type wrapping the exception
				return instance<>();
			}
		}
	};
}}
