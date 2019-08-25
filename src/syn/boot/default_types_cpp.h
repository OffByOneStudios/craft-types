#pragma once
#include "syn/syn.h"

namespace syn
{
    template<> struct type_define<bool> { static syn::Define<bool> Definition; };

    template<> struct type_define<::std::string> { static syn::Define<::std::string> Definition; };
}
