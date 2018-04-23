#include "../common.h"

#include "default_types_c.h"

CRAFT_TYPE_DEFINE(void*)
{
	_.add<GraphPropertyName>("ptr");
	_.add<GraphPropertyCppName>("void*");
};

CRAFT_TYPE_DEFINE(int8_t)
{
	_.add<GraphPropertyName>("int8");
	_.add<GraphPropertyCppName>("int8_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int8_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(int16_t)
{
	_.add<GraphPropertyName>("int16");
	_.add<GraphPropertyCppName>("int16_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int16_t>::make(int16_t(std::stol(s))); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();

};
CRAFT_TYPE_DEFINE(int32_t)
{
	_.add<GraphPropertyName>("int32");
	_.add<GraphPropertyCppName>("int32_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(int64_t)
{
	_.add<GraphPropertyName>("int64");
	_.add<GraphPropertyCppName>("int64_t");

	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<int64_t>::make(std::stol(s)); } );
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<int64_t> _this) { return std::to_string(*_this); } );
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int64_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};

CRAFT_TYPE_DEFINE(uint8_t)
{
	_.add<GraphPropertyName>("uint8");
	_.add<GraphPropertyCppName>("uint8_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint8_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(uint16_t)
{
	_.add<GraphPropertyName>("uint16");
	_.add<GraphPropertyCppName>("uint16_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint16_t>::make(uint16_t(std::stoi(s))); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();

};
CRAFT_TYPE_DEFINE(uint32_t)
{
	_.add<GraphPropertyName>("uint32");
	_.add<GraphPropertyCppName>("uint32_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(uint64_t)
{
	_.add<GraphPropertyName>("uint64");
	_.add<GraphPropertyCppName>("uint64_t");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint64_t>::make(std::stol(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};

CRAFT_TYPE_DEFINE(float)
{
	_.add<GraphPropertyName>("float32");
	_.add<GraphPropertyCppName>("float");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<float>::make(std::stof(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<float> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<float> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(double)
{
	_.add<GraphPropertyName>("float64");
	_.add<GraphPropertyCppName>("double");

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<double>::make(std::stod(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<double> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<double> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
