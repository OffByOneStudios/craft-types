#include "syn/syn.h"
#include "default_types_c.h"

// CRAFT_TYPE_DEFINE(void*)
// {
// 	_.identify_byFullCppName("void*");
// 	_.identify("syndicate.pointer");
// 	_.prop<Type_Property_NativeSize>({ sizeof(void*) });
// };

// CRAFT_TYPE_DEFINE(int8_t)
// {
// 	_.identify_byFullCppName("int8_t");
// 	_.identify("syndicate.i8");
// 	_.prop<Type_Property_NativeSize>({ sizeof(int8_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int8_t>::make(std::stoi(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int8_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };
// CRAFT_TYPE_DEFINE(int16_t)
// {
// 	_.identify_byFullCppName("int16_t");
// 	_.identify("syndicate.i16");
// 	_.prop<Type_Property_NativeSize>({ sizeof(int16_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int16_t>::make(int16_t(std::stol(s))); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int16_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();

// };
// CRAFT_TYPE_DEFINE(int32_t)
// {
// 	_.identify_byFullCppName("int32_t");
// 	_.identify("syndicate.i32");
// 	_.prop<Type_Property_NativeSize>({ sizeof(int32_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<int32_t>::make(std::stoi(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int32_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };
// CRAFT_TYPE_DEFINE(int64_t)
// {
// 	_.identify_byFullCppName("int64_t");
// 	_.identify("syndicate.i64");
// 	_.prop<Type_Property_NativeSize>({ sizeof(int64_t) });

// 	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<int64_t>::make(std::stol(s)); } );
// 	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<int64_t> _this) { return std::to_string(*_this); } );
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<int64_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };

// CRAFT_TYPE_DEFINE(uint8_t)
// {
// 	_.identify_byFullCppName("uint8_t");
// 	_.identify("syndicate.u8");
// 	_.prop<Type_Property_NativeSize>({ sizeof(uint8_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint8_t>::make(std::stoi(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint8_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };
// CRAFT_TYPE_DEFINE(uint16_t)
// {
// 	_.identify_byFullCppName("uint16_t");
// 	_.identify("syndicate.u16");
// 	_.prop<Type_Property_NativeSize>({ sizeof(uint16_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint16_t>::make(uint16_t(std::stoi(s))); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint16_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();

// };
// CRAFT_TYPE_DEFINE(uint32_t)
// {
// 	_.identify_byFullCppName("uint32_t");
// 	_.identify("syndicate.u32");
// 	_.prop<Type_Property_NativeSize>({ sizeof(uint32_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint32_t>::make(std::stoi(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint32_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };
// CRAFT_TYPE_DEFINE(uint64_t)
// {
// 	_.identify_byFullCppName("uint64_t");
// 	_.identify("syndicate.u64");
// 	_.prop<Type_Property_NativeSize>({ sizeof(uint64_t) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<uint64_t>::make(std::stol(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<uint64_t> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };

// CRAFT_TYPE_DEFINE(float)
// {
// 	_.identify_byFullCppName("float");
// 	_.identify("syndicate.f32");
// 	_.prop<Type_Property_NativeSize>({ sizeof(float) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<float>::make(std::stof(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<float> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<float> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };
// CRAFT_TYPE_DEFINE(double)
// {
// 	_.identify_byFullCppName("double");
// 	_.identify("syndicate.f64");
// 	_.prop<Type_Property_NativeSize>({ sizeof(double) });

// 	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { return instance<double>::make(std::stod(s)); });
// 	_.use<PStringer>().singleton<FunctionalStringer>([](::craft::instance<double> _this) { return std::to_string(*_this); });
// 	_.use<PRepr>().singleton<FunctionalRepr>([](::craft::instance<double> _this) { return std::to_string(*_this); });
// 	_.use<PConstructor>().singleton<DefaultConstructor>();
// };

using namespace syn;

decltype(syn::type_define<uint8_t>::Definition) syn::type_define<uint8_t>::Definition(
	[](auto _) {
		_.name("UInt8");
        _.subtypes(core::Unsigned);
	});

decltype(syn::type_define<uint16_t>::Definition) syn::type_define<uint16_t>::Definition(
	[](auto _) {
		_.name("UInt16");
        _.subtypes(core::Unsigned);
	});

decltype(syn::type_define<uint32_t>::Definition) syn::type_define<uint32_t>::Definition(
	[](auto _) {
		_.name("UInt32");
        _.subtypes(core::Unsigned);
	});

decltype(syn::type_define<uint64_t>::Definition) syn::type_define<uint64_t>::Definition(
	[](auto _) {
		_.name("UInt64");
        _.subtypes(core::Unsigned);
	});


decltype(syn::type_define<int8_t>::Definition) syn::type_define<int8_t>::Definition(
	[](auto _) {
		_.name("Int8");
        _.subtypes(core::Signed);
	});

decltype(syn::type_define<int16_t>::Definition) syn::type_define<int16_t>::Definition(
	[](auto _) {
		_.name("Int16");
        _.subtypes(core::Signed);
	});

decltype(syn::type_define<int32_t>::Definition) syn::type_define<int32_t>::Definition(
	[](auto _) {
		_.name("Int32");
        _.subtypes(core::Signed);
	});

decltype(syn::type_define<int64_t>::Definition) syn::type_define<int64_t>::Definition(
	[](auto _) {
		_.name("Int64");
        _.subtypes(core::Signed);
	});


decltype(syn::type_define<float>::Definition) syn::type_define<float>::Definition(
	[](auto _) {
		_.name("float");
        _.subtypes(core::Floating);
	});

decltype(syn::type_define<double>::Definition) syn::type_define<double>::Definition(
	[](auto _) {
		_.name("double");
        _.subtypes(core::Floating);
	});
