#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{

	/******************************************************************************
	** PIdentifier
	******************************************************************************/

	/* T:
	Provides an interface for naming things, and reversing those names into actual objects.
	*/

	class PIdentifier abstract
		: public Provider
	{
		CRAFT_TYPES_EXPORTED CRAFT_LEGACY_FEATURE_DECLARE(PIdentifier, "types.id", NamedSingletonProviderManager);

	public:
		virtual std::string identifier() const = 0;

	public:
		inline std::string provider_index() const { return identifier(); }
	};

	/******************************************************************************
	** SimpleIdentifier
	******************************************************************************/

	template <typename T>
	class SimpleIdentifier
		: public Implements<PIdentifier>::For<T>
	{
		std::string _name;

	public:
		inline SimpleIdentifier(std::string const& name): _name(name) { }

		inline virtual std::string identifier() const override { return _name; }
	};

	/******************************************************************************
	** DefaultIdentifier
	******************************************************************************/

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
			auto end = std::remove_if(_parts.begin(), _parts.end(), [](std::string i) {
				return i.size() == 0;
			});

			return stdext::join('.', _parts.begin(), end);
		}

	public:
		inline DefaultIdentifier() { _typename = _process(T::craft_s_typeName()); }

		inline virtual std::string identifier() const override { return _typename; }
	};
}}
