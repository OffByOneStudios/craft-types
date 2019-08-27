#pragma once
#include "syn/syn.h"

namespace syn
{
    template<> struct type_define<bool> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<bool> Definition; };

    template<> struct type_define<::std::string> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::std::string> Definition; };
}
