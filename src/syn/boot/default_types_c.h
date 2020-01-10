#pragma once
#include "syn/syn.h"

namespace syn
{
    template<> struct type_define<uint8_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<uint8_t> Definition; };
    template<> struct type_define<uint16_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<uint16_t> Definition; };
    template<> struct type_define<uint32_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<uint32_t> Definition; };
    template<> struct type_define<uint64_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<uint64_t> Definition; };

    template<> struct type_define<int8_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<int8_t> Definition; };
    template<> struct type_define<int16_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<int16_t> Definition; };
    template<> struct type_define<int32_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<int32_t> Definition; };
    template<> struct type_define<int64_t> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<int64_t> Definition; };

    template<> struct type_define<float> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<float> Definition; };
    template<> struct type_define<double> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<double> Definition; };
}
