#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/system/TypeStore.h"

TEST_CASE( "syn::TypeStore (making our own)", "[syn::TypeStore]" )
{
    syn::TypeStore store;

    SECTION( ".g() accesses the graph (and it is empty by default)" )
    {
        auto g = store.g();

        CHECK(g.nodeCount() == 0);
    }

    SECTION( ".s() accesses the symbol table (and it is empty by default)" )
    {
        auto s = store.s();

        CHECK(s.count() == 0);
    }
}
