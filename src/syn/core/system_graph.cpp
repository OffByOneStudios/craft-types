
#include "syn/syn.h"
#include "system_graph.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;


decltype(syn::type_define<::syn::core::NEmpty>::Definition) syn::type_define<::syn::core::NEmpty>::Definition(
	[](auto _) {
		_.name("Empty");
	});

decltype(syn::type_define<::syn::core::NAbstract>::Definition) syn::type_define<::syn::core::NAbstract>::Definition(
	[](auto _) {
		_.name("Abstract");
	});

decltype(syn::type_define<::syn::core::EIsA>::Definition) syn::type_define<::syn::core::EIsA>::Definition(
	[](auto _) {
		_.name("Empty");
	});

decltype(syn::core::MetaDefinitions) syn::core::MetaDefinitions(
	[](auto _) {

	});


decltype(syn::type_define<::syn::core::NBits>::Definition) syn::type_define<::syn::core::NBits>::Definition(
	[](auto _) {
		_.name("Bits");
	});

decltype(syn::type_define<::syn::core::NStruct>::Definition) syn::type_define<::syn::core::NStruct>::Definition(
	[](auto _) {
		_.name("Struct");
	});

decltype(syn::type_define<::syn::core::NReference>::Definition) syn::type_define<::syn::core::NReference>::Definition(
	[](auto _) {
		_.name("Reference");
	});

decltype(syn::core::VoidAndPointerDefinitions) syn::core::VoidAndPointerDefinitions(
	[](auto _) {

	});


decltype(syn::type_define<::syn::core::NSignature>::Definition) syn::type_define<::syn::core::NSignature>::Definition(
	[](auto _) {
		_.name("Signature");
	});

decltype(syn::type_define<::syn::core::NFunction>::Definition) syn::type_define<::syn::core::NFunction>::Definition(
	[](auto _) {
		_.name("Function");
	});
