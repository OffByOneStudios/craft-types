#include "../common.h"

#include "default_types_c.h"

CRAFT_TYPE_DEFINE(void*)
{
	_.identify_verbose("ptr", "ptr", "void*");
	_.add<GraphPropertyCppSize>(sizeof(void*));
};

CRAFT_TYPE_DEFINE(int8_t)
{
	_.identify_verbose("int8", "int8", "int8_t");
	_.add<GraphPropertyCppSize>(sizeof(int8_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int8_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(int16_t)
{
	_.identify_verbose("int16", "int16", "int16_t");
	_.add<GraphPropertyCppSize>(sizeof(int16_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int16_t>::make(int16_t(std::stol(s))); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();

};
CRAFT_TYPE_DEFINE(int32_t)
{
	_.identify_verbose("int32", "int32", "int32_t");
	_.add<GraphPropertyCppSize>(sizeof(int32_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(int64_t)
{
	_.identify_verbose("int64", "int64", "int64_t");
	_.add<GraphPropertyCppSize>(sizeof(int64_t));

	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<int64_t>::make(std::stol(s)); } );
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<int64_t> _this) { return std::to_string(*_this); } );
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int64_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};

CRAFT_TYPE_DEFINE(uint8_t)
{
	_.identify_verbose("uint8", "uint8", "uint8_t");
	_.add<GraphPropertyCppSize>(sizeof(uint8_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint8_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(uint16_t)
{
	_.identify_verbose("uint16", "uint16", "uint16_t");
	_.add<GraphPropertyCppSize>(sizeof(uint16_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint16_t>::make(uint16_t(std::stoi(s))); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();

};
CRAFT_TYPE_DEFINE(uint32_t)
{
	_.identify_verbose("uint32", "uint32", "uint32_t");
	_.add<GraphPropertyCppSize>(sizeof(uint32_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint32_t>::make(std::stoi(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(uint64_t)
{
	_.identify_verbose("uint64", "uint64", "uint64_t");
	_.add<GraphPropertyCppSize>(sizeof(uint64_t));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint64_t>::make(std::stol(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};

CRAFT_TYPE_DEFINE(float)
{
	_.identify_verbose("float32", "float32", "float");
	_.add<GraphPropertyCppSize>(sizeof(float));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<float>::make(std::stof(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<float> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<float> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
CRAFT_TYPE_DEFINE(double)
{
	_.identify_verbose("float64", "float64", "double");
	_.add<GraphPropertyCppSize>(sizeof(double));

	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<double>::make(std::stod(s)); });
	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<double> _this) { return std::to_string(*_this); });
	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<double> _this) { return std::to_string(*_this); });
	_.use<PConstructor>().singleton<DefaultConstructor>();
};
