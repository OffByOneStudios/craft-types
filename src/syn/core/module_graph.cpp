
#include "syn/syn.h"
#include "cpp_graph.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;


decltype(syn::type_define<::syn::core::NModule>::Definition) syn::type_define<::syn::core::NModule>::Definition(
	[](auto _) {
		_.name("Module");
	});

decltype(syn::type_define<::syn::core::PModuleSymbol>::Definition) syn::type_define<::syn::core::PModuleSymbol>::Definition(
	[](auto _) {
		_.name("ModuleSymbol");
	});
