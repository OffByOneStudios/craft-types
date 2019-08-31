
#include "syn/syn.h"
#include "numerics.h"

using namespace syn;
using namespace syn::core;

decltype(syn::core::Numeric) syn::core::Numeric(
	[](auto _) {
		_.name("Numeric");
	});

decltype(syn::core::Integral) syn::core::Integral(
	[](auto _) {
		_.name("Integral");
        _.subtypes(Numeric);
	});

decltype(syn::core::Signed) syn::core::Signed(
	[](auto _) {
		_.name("Signed");
        _.subtypes(Integral);
	});

decltype(syn::core::Unsigned) syn::core::Unsigned(
	[](auto _) {
		_.name("Unsigned");
        _.subtypes(Integral);
	});

decltype(syn::core::Floating) syn::core::Floating(
	[](auto _) {
		_.name("Floating");
        _.subtypes(Numeric);
	});

decltype(syn::core::add) syn::core::add(
	[](auto _) {
		_.name("add");
	});

decltype(syn::core::sub) syn::core::sub(
	[](auto _) {
		_.name("sub");
	});

decltype(syn::core::mul) syn::core::mul(
	[](auto _) {
		_.name("mul");
	});

decltype(syn::core::div) syn::core::div(
	[](auto _) {
		_.name("div");
	});
