#include "../common.h"

#include "../core.h"
#include "default_types_cpp.h"

// stdlib
CRAFT_TYPE_DEFINE(std::string)
{
	_.identify_verbose("string", "String", "std::string");

	_.use<PClone>().singleton<DefaultCopyConstructor>();
	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<std::string>::make(s); });
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<std::string> _this) { return  *_this; } );
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<std::string> _this) { return fmt::format("\"{0}\"", *_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

// stdlib
CRAFT_TYPE_DEFINE(bool)
{
	_.identify_verbose("boolean", "Bool", "bool");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s)
	{
		if (s == "true" || s == "True")
		{
			return instance<bool>::make(true);
		}
		else if (s == "false" || s == "False")
		{
			return instance<bool>::make(false);
		}
		else
		{
			throw stdext::exception(fmt::format("{0} is not corecable to type Bool", s));
		}
	});
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<bool> _this) 
	{ 
		return (*_this) ? "true" : "false";
	});
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<bool> _this) { return (*_this) ? "true" : "false"; });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
