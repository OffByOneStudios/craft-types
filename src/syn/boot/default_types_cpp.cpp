#include "syn/syn.h"
#include "default_types_cpp.h"

// stdlib
decltype(syn::type_define<::std::string>::Definition) syn::type_define<::std::string>::Definition(
	[](auto _) {
		_.name("string");
	});

decltype(syn::type_define<bool>::Definition) syn::type_define<bool>::Definition(
	[](auto _) {
		_.name("bool");
	});

