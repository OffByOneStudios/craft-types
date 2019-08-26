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
    test_require_syn_boot();
    
    SECTION( "starts empty" )
    {
        instance<std::string> inst;

        CHECK(inst.get() == nullptr);
        CHECK(inst.isNull() == true);
        CHECK((bool)inst == false);
    }

    SECTION( "is a smart pointer" )
    {
        instance<std::string> inst;
        inst = instance<std::string>::make("hello");

        CHECK(inst.isNull() == false);
        CHECK((bool)inst == true);
        REQUIRE(inst.get() != nullptr);
        CHECK(inst.typeId() == syn::type<std::string>::id());

        CHECK(*inst.get() == "hello");
        CHECK(*inst.get() == std::string("hello"));

        CHECK(*inst == "hello");
        CHECK(*inst == std::string("hello"));

        CHECK(inst->size() == std::string("hello").size());
    }

    SECTION( "smart pointer with type erase" )
    {
        instance<> inst;
        inst = instance<std::string>::make("hello");

        CHECK(inst.isNull() == false);
        CHECK((bool)inst == true);
        REQUIRE(inst.get() != nullptr);
        CHECK(inst.typeId() == syn::type<std::string>::id());
    }
}
