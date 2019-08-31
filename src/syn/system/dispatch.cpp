#include "syn/syn.h"
#include "dispatch.h"

using namespace syn;

bool syn::is_a(TypeId most_specific, TypeId less_specific)
{
    if (most_specific == less_specific)
        return true;
    if (most_specific == 0 || less_specific == 0)
        return false;

    // TODO sort this and save to cache
    auto res = graph::query(&thread_store().g())
        .v(most_specific)
        .repeat_until(
            [](auto sq) { return sq
                .in([](auto e) { return e->type == type<core::EIsA>::id(); })
                .unique()
                ; },
            [&](auto n) { n == less_specific; }
        )
        .run();

    return most_specific == less_specific;
}

TypeId syn::basic_dispatch(TypeId dispatcher, TypeId* type_args, size_t count, void* value_args /* = nullptr */, TypeId previous_call /* = nullptr */)
{
    // TODO save this to a cache
    std::vector<std::vector<TypeId>> dispatch_table_by_arg;
    for (int i = 0; i < count; ++i)
    {

    }


}
