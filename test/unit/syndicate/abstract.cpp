#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/system/dispatch.h"

using namespace syn;
using namespace graph;

TEST_CASE( "abstracts & empty", "[syndicate/core]" )
{
    // TODO move this to it's own "with booted syndicate" project
    test_require_syn_boot();

    auto& g = global_store().g();
    auto& s = global_store().s();

    SECTION( "empty" )
    {
        CHECK(s.getSymbol("Empty"));
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
