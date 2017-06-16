#include "common.h"

#include "core.h"
#include "default_types_cpp.h"

// stdlib
CRAFT_TYPE_DEFINE(std::string)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("std.string");
	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<std::string>::make(s); });
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<std::string> _this) { return *_this; } );
}

// stdlib
CRAFT_TYPE_DEFINE(bool)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("bool");
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
		return (_this.get()) ? "true" : "false";
	});
}
