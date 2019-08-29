
#include "syn/syn.h"
#include "syndicate_graph.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;


decltype(syn::type_define<::syn::core::NDispatcher>::Definition) syn::type_define<::syn::core::NDispatcher>::Definition(
	[](auto _) {
		_.name("Dispatcher");
	});

decltype(syn::type_define<::syn::core::PCompositionalCast>::Definition) syn::type_define<::syn::core::PCompositionalCast>::Definition(
	[](auto _) {
		_.name("CompositionalCast");
	});
