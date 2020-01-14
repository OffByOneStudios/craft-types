#include "syn/syn.h"
#include "default_types_cpp.h"

// stdlib
decltype(syn::type_define<::std::string>::Definition) syn::type_define<::std::string>::Definition(
	[](auto _) {
		_.name("String");
	});

decltype(syn::type_define<bool>::Definition) syn::type_define<bool>::Definition(
	[](auto _) {
		_.name("Bool");
	});

