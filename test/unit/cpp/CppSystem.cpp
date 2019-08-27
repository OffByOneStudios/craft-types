#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/cpp/cpp_interface.h"

using namespace syn;

TEST_CASE( "type nodes inited", "[syn::CppSystem]" )
{
    // Can't really do pre checks, we have no shutdown system :/
    test_require_syn_boot();

    SECTION( "std::string post" )
    {
        CHECK(syn::type<std::string>::desc() != nullptr);
        CHECK((uintptr_t)syn::type<std::string>::id() != 0);
        CHECK(syn::type<std::string>::graphNode() != nullptr);
    }
}
