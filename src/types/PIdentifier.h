#pragma once
#include "common.h"

namespace craft {

	/******************************************************************************
	** PIdentifier
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PIdentifier abstract
		: public Provider
	{
		CRAFT_PROVIDER_DECLARE(PIdentifier, "types.id", NamedSingleton)
	public:
		virtual std::string identifier() const = 0;

	public:
		inline std::string provider_index() const { return identifier(); }
	};

	/******************************************************************************
	** DefaultIdentifier
	******************************************************************************/

	namespace _details
	{
		template <typename T>
		class DefaultIdentifier
			: public Implements<PIdentifier>::For<T>
		{
			std::string _typename;

			inline std::string _process(std::string const& s)
			{
				std::string result;

				if (s.find("::") == std::string::npos)
					return "." + s;

				std::vector<std::string> _parts;
				stdext::split(s, "::", std::back_inserter(_parts));
				auto end = std::remove_if(_parts.begin(), _parts.end(), std::bind(&std::string::empty, _1));

				return stdext::join('.', _parts.begin(), end);
			}

		public:
			inline DefaultIdentifier() { _typename = _process(T::craft_s_typeName()); }

			inline virtual std::string identifier() const override { return _typename; }
		};
	}
}
