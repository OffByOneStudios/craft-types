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

