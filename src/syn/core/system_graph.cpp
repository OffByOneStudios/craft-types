
#include "syn/syn.h"
#include "system_graph.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;
using namespace syn::core;


decltype(syn::type_define<::syn::core::NEmpty>::Definition) syn::type_define<::syn::core::NEmpty>::Definition(
	[](auto _) {
		_.name("Empty");
		_.subtypes(MetaNode);
	});

decltype(syn::type_define<::syn::core::NAbstract>::Definition) syn::type_define<::syn::core::NAbstract>::Definition(
	[](auto _) {
		_.name("Abstract");
		_.subtypes(MetaNode);
	});

decltype(syn::type_define<::syn::core::EIsA>::Definition) syn::type_define<::syn::core::EIsA>::Definition(
	[](auto _) {
		_.name("IsA");
		_.subtypes(MetaEdge);
	});

decltype(syn::core::Meta) syn::core::Meta(
	[](auto _) {
		_.name("Meta");
	});

decltype(syn::core::MetaNode) syn::core::MetaNode(
	[](auto _) {
		_.name("MetaNode");
		_.subtypes(Meta);
	});
decltype(syn::core::MetaEdge) syn::core::MetaEdge(
	[](auto _) {
		_.name("MetaEdge");
		_.subtypes(Meta);
	});
decltype(syn::core::MetaLabel) syn::core::MetaLabel(
	[](auto _) {
		_.name("MetaLabel");
		_.subtypes(MetaLabel);
	});
decltype(syn::core::MetaProp) syn::core::MetaProp(
	[](auto _) {
		_.name("MetaProp");
		_.subtypes(MetaProp);
	});

decltype(syn::core::Type) syn::core::Type(
	[](auto _) {
		_.name("Type");
	});
decltype(syn::core::Subroutine) syn::core::Subroutine(
	[](auto _) {
		_.name("Subroutine");
	});

decltype(syn::type_define<::syn::core::NBits>::Definition) syn::type_define<::syn::core::NBits>::Definition(
	[](auto _) {
		_.name("Bits");
		_.subtypes(MetaNode);
		_.subtypes(Type);
	});

decltype(syn::type_define<::syn::core::NStruct>::Definition) syn::type_define<::syn::core::NStruct>::Definition(
	[](auto _) {
		_.name("Struct");
		_.subtypes(MetaNode);
		_.subtypes(Type);
	});

decltype(syn::type_define<::syn::core::NReference>::Definition) syn::type_define<::syn::core::NReference>::Definition(
	[](auto _) {
		_.name("Reference");
		_.subtypes(MetaNode);
		_.subtypes(Type);
	});

decltype(syn::core::VoidAndPointerDefinitions) syn::core::VoidAndPointerDefinitions(
	[](auto _) {

	});


decltype(syn::type_define<::syn::core::NSignature>::Definition) syn::type_define<::syn::core::NSignature>::Definition(
	[](auto _) {
		_.name("Signature");
		_.subtypes(MetaNode);
	});

decltype(syn::type_define<::syn::core::NFunction>::Definition) syn::type_define<::syn::core::NFunction>::Definition(
	[](auto _) {
		_.name("Function");
		_.subtypes(MetaNode);
		_.subtypes(Subroutine);
	});
