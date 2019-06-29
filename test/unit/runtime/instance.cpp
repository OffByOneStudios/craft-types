#include "catch2/catch.hpp"

#include "unit/shared.h"

#include "syn/syn.h"
#include "syn/runtime/instance.h"

using namespace syn;

TEST_CASE( "syn::InstanceLifecycle", "[syn::InstanceLifecycle]" )
{
    SECTION( "flag opperations" )
    {
        InstanceLifecycle l = InstanceLifecycle::ReferenceCounted(15);

        CHECK(
            (l &= InstanceLifecycle::ManagerUse)
            == false);

        l = l | InstanceLifecycle::ManagerUse;

        CHECK(
            (l &= InstanceLifecycle::ManagerUse)
            == true);
    }

    SECTION( "mode operations" )
    {
        InstanceLifecycle l = InstanceLifecycle(15);

        CHECK(
            (l &= InstanceLifecycle::ModeReferenceCounted)
            == false);

        l = l | InstanceLifecycle::ModeReferenceCounted;

        CHECK(
            (l &= InstanceLifecycle::ModeReferenceCounted)
            == true);
    }

    SECTION( "deleter operations" )
    {
        InstanceLifecycle l = InstanceLifecycle(15);

        CHECK(
            (l &= InstanceLifecycle::DeleterHeader)
            == false);

        l = l | InstanceLifecycle::DeleterHeader;

        CHECK(
            (l &= InstanceLifecycle::DeleterHeader)
            == true);
    }

    SECTION( "flag opperations can test for modes" )
    {
        InstanceLifecycle l = InstanceLifecycle::ReferenceCounted(15);

        CHECK(
            (l &= InstanceLifecycle::Mask_Deleter)
            == false);

        l = l | InstanceLifecycle::DeleterDirect;

        CHECK(
            (l &= InstanceLifecycle::Mask_Deleter)
            == true);
    }

    SECTION( "data operations work" )
    {
        InstanceLifecycle l = InstanceLifecycle::ReferenceCounted(15) | InstanceLifecycle::DeleterDirect;

        CHECK(*l == 15);

        *l = 42;

        CHECK(*l == 42);
    }
}
