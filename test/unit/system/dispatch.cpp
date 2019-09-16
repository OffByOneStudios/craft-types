#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/system/dispatch.h"

using namespace syn;

TEST_CASE( "is-a", "[system]" )
{
    // Can't really do pre checks, we have no shutdown system :/
    test_require_syn_boot();

    SECTION( "equal types" )
    {
        CHECK(syn::is_a(syn::type<std::string>::id(), syn::type<std::string>::id()));
        CHECK(!syn::is_a(syn::type<std::string>::id(), None));
        CHECK(!syn::is_a(None, syn::type<std::string>::id()));
    }

    SECTION( "abstract inheritence" )
    {
        CHECK(syn::is_a(syn::type<uint64_t>::id(), syn::type<uint64_t>::id()));

        CHECK(syn::is_a(syn::type<uint64_t>::id(), syn::core::Unsigned));
        CHECK(!syn::is_a(syn::type<uint64_t>::id(), syn::core::Signed));

        CHECK(syn::is_a(syn::type<uint64_t>::id(), syn::core::Integral));
        
        CHECK(syn::is_a(syn::type<uint64_t>::id(), syn::core::Numeric));
    }
}
