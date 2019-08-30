
#include "syn/syn.h"
#include "conversions.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;
using namespace syn::core;

decltype(syn::core::parse) syn::core::parse(
	[](auto _) {
		_.name("parse");

        _.method([](TypeId type, instance<syn::String> str){
            return instance<>();
        });
	});
