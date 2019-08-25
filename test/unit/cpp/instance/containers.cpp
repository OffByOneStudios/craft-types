#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/cpp/instance/containers.hpp"

using namespace syn;

TEST_CASE( "syn::instance<void>", "[syn::CppReferenceCounted]" )
{
    SECTION( "starts empty" )
    {
        instance<> inst;

        CHECK(inst.get() == nullptr);
        CHECK(inst.isNull() == true);
        CHECK(inst == false);
    }
}

TEST_CASE( "syn::instance<T>", "[syn::CppReferenceCounted]" )
{
    SECTION( "starts empty" )
    {
        instance<std::string> inst;

        CHECK(inst.get() == nullptr);
        CHECK(inst.isNull() == true);
        CHECK(inst == false);
    }

    SECTION( "can be made" )
    {
        instance<std::string> inst;

        CHECK(inst.get() == nullptr);
        CHECK(inst.isNull() == true);
        CHECK(inst == false);
    }
}
