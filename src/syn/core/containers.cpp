
#include "syn/syn.h"
#include "containers.h"
#include "syn/boot/system_into_cpp.h"

using namespace syn;
using namespace syn::core;

decltype(syn::core::AbstractContainer) syn::core::AbstractContainer(
	[](auto _) {
		_.name("AbstractContainer");
	});

decltype(syn::core::count) syn::core::count(
	[](auto _) {
		_.name("count");
	});

decltype(syn::core::AbstractVector) syn::core::AbstractVector(
	[](auto _) {
		_.name("AbstractVector");
        _.subtypes(AbstractContainer);
	});

decltype(syn::type_define<::syn::core::Vector>::Definition) syn::type_define<::syn::core::Vector>::Definition(
	[](auto _) {
		_.name("Vector");
        _.subtypes(AbstractVector);

        _.template method<&Vector::size>(count);
	});

decltype(syn::type_define<::syn::core::ByteVector>::Definition) syn::type_define<::syn::core::ByteVector>::Definition(
	[](auto _) {
		_.name("ByteVector");
        _.subtypes(AbstractVector);
        
        _.template method<&ByteVector::size>(count);
	});

decltype(syn::core::AbstractDictionary) syn::core::AbstractDictionary(
	[](auto _) {
		_.name("AbstractDictionary");
        _.subtypes(AbstractContainer);
	});

decltype(syn::type_define<::syn::core::Dictionary>::Definition) syn::type_define<::syn::core::Dictionary>::Definition(
	[](auto _) {
		_.name("Dictionary");
        _.subtypes(AbstractDictionary);

        _.template method<&Dictionary::size>(count);
	});

decltype(syn::type_define<::syn::core::StringDictionary>::Definition) syn::type_define<::syn::core::StringDictionary>::Definition(
	[](auto _) {
		_.name("StringDictionary");
        _.subtypes(AbstractDictionary);

        _.template method<&StringDictionary::size>(count);
	});

decltype(syn::type_define<::syn::core::SymbolDictionary>::Definition) syn::type_define<::syn::core::SymbolDictionary>::Definition(
	[](auto _) {
		_.name("SymbolDictionary");
        _.subtypes(AbstractDictionary);

        _.template method<&SymbolDictionary::size>(count);
	});

decltype(syn::core::AbstractSet) syn::core::AbstractSet(
	[](auto _) {
		_.name("AbstractSet");
        _.subtypes(AbstractContainer);
	});

decltype(syn::type_define<::syn::core::Set>::Definition) syn::type_define<::syn::core::Set>::Definition(
	[](auto _) {
		_.name("Set");
        _.subtypes(AbstractSet);

        _.template method<&Set::size>(count);
	});
