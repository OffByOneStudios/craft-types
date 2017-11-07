#include "common.h"

#include "default_types_c.h"

CRAFT_TYPE_DEFINE(int32_t)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("int32");
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_TYPE_DEFINE(int64_t)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("int64");
	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<int64_t>::make(std::stol(s)); } );
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<int64_t> _this) { return std::to_string(*_this); } );
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(uint32_t)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("uint32");
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_TYPE_DEFINE(uint64_t)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("uint64");
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint64_t>::make(std::stol(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}

CRAFT_TYPE_DEFINE(float)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("float32");
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<float>::make(std::stof(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<float> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
CRAFT_TYPE_DEFINE(double)
{
	_.use<PIdentifier>().singleton<SimpleIdentifier>("float64");
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<double>::make(std::stod(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<double> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
}
