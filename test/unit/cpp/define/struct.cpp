#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/cpp/DefineHelper.hpp"

using namespace syn;

TEST_CASE( "syn::DefineHelperLibraryNaming", "[syn::DefineHelper]" )
{
    test_require_syn_boot();

}

struct StructureBase
{
    uint32_t foo;
    uint32_t bar;
};

struct StructureDerived
    : public StructureBase
{
    uint32_t baz;
    StructureBase boop;
    uint64_t lots[15];
};

TEST_CASE( "syn::DefineHelperLibraryStructure", "[syn::DefineHelper]" )
{
    test_require_syn_boot();
    
    SECTION( "inherits<T>" )
    {
        syn::Define<StructureBase> baseDefine([](auto _){

        });

        syn::Define<StructureDerived> deriveDefine([](auto _){

        });

        syn::dll::update();
    }
}
