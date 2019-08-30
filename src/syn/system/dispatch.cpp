#include "syn/syn.h"
#include "dispatch.h"

using namespace syn;

bool syn::is_a(TypeId most_specific, TypeId less_specific)
{
    auto res = graph::query(&thread_store().g())
        .v(most_specific)
        .in([](auto e) { return e->type == type<core::EIsA>::id(); })
        .run();

    return most_specific == less_specific;
}

TypeId syn::basic_dispatch(TypeId dispatcher, TypeId* type_args, size_t count, void* value_args /* = nullptr */, TypeId previous_call /* = nullptr */)
{
    return nullptr;
}
