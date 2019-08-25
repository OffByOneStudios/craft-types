#pragma once
#include "syn/syn.h"

/* This file fixes up the system by declaring the system types as external cpp types.

	See the relevant .cpp files for the defines
*/

/******************************************************************************
** /syn/core/system_graph.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::NEmpty> { static syn::Define<::syn::core::NEmpty> Definition; };
    template<> struct type_define<::syn::core::NAbstract> { static syn::Define<::syn::core::NAbstract> Definition; };
    template<> struct type_define<::syn::core::EIsA> { static syn::Define<::syn::core::EIsA> Definition; };
	namespace core { extern syn::Define<> MetaDefinitions; }

    template<> struct type_define<::syn::core::NBits> { static syn::Define<::syn::core::NBits> Definition; };
    template<> struct type_define<::syn::core::NStruct> { static syn::Define<::syn::core::NStruct> Definition; };
    template<> struct type_define<::syn::core::NReference> { static syn::Define<::syn::core::NReference> Definition; };
	namespace core { extern syn::Define<> VoidAndPointerDefinitions; }

    template<> struct type_define<::syn::core::NSignature> { static syn::Define<::syn::core::NSignature> Definition; };
    template<> struct type_define<::syn::core::NFunction> { static syn::Define<::syn::core::NFunction> Definition; };
}

/******************************************************************************
** /syn/core/cpp_graph.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::PCppDefine> { static syn::Define<::syn::core::PCppDefine> Definition; };
}
