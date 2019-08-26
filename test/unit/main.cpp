#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

#include "shared.h"

#include <mutex>

#include "syn/syn.h"

std::once_flag test_require_syn;

void test_require_syn_boot()
{
    std::call_once(test_require_syn, []()
    {
        syn::dll::boot();
    });
}
