
#include "syn/syn.h"
#include "conversions.h"

using namespace syn;
using namespace syn::core;

decltype(syn::core::parse) syn::core::parse(
	[](auto _) {
		_.name("parse");

        _.method([](TypeId type, instance<std::string> str){
            return instance<>();
        });
	});
