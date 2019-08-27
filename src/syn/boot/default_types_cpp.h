#pragma once
#include "syn/syn.h"

namespace syn
{
    template<> struct type_define<bool> { CRAFT_TYPES_EXPORTED static syn::Define<bool> Definition; };

    template<> struct type_define<::std::string> { CRAFT_TYPES_EXPORTED static syn::Define<::std::string> Definition; };
}
