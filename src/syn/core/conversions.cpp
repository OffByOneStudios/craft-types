
#include "syn/syn.h"
#include "conversions.h"

using namespace syn;
using namespace syn::core;

decltype(syn::core::promote) syn::core::promote(
	[](auto _) {
		_.name("promote");

        // If we dispatch here, we have no commonality
        _.method([](instance<> a, instance<> b){
            return instance<>();
        });
	});


decltype(syn::core::parse) syn::core::parse(
	[](auto _) {
		_.name("parse");

        _.method([](TypeId type, instance<std::string> str){
            return instance<>();
        });
	});
