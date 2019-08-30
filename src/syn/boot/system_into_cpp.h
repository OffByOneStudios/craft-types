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
    template<> struct type_define<::syn::core::NEmpty> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NEmpty> Definition; };
    template<> struct type_define<::syn::core::NAbstract> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NAbstract> Definition; };
    template<> struct type_define<::syn::core::EIsA> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::EIsA> Definition; };
	namespace core
    {
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract Meta;
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract MetaNode;
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract MetaEdge;
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract MetaLabel;
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract MetaProp;

        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract Type;
        CULTLANG_SYNDICATE_EXPORTED extern syn::Abstract Subroutine;
    }

    template<> struct type_define<::syn::core::NBits> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NBits> Definition; };
    template<> struct type_define<::syn::core::NStruct> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NStruct> Definition; };
    template<> struct type_define<::syn::core::NReference> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NReference> Definition; };
	namespace core { extern syn::Define<> VoidAndPointerDefinitions; }

    template<> struct type_define<::syn::core::NSignature> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NSignature> Definition; };
    template<> struct type_define<::syn::core::NFunction> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NFunction> Definition; };
}

/******************************************************************************
** /syn/core/syndicate_graph.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::NDispatcher> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NDispatcher> Definition; };
    template<> struct type_define<::syn::core::PCompositionalCast> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::PCompositionalCast> Definition; };
}

/******************************************************************************
** /syn/core/module_graph.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::NModule> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::NModule> Definition; };

    template<> struct type_define<::syn::core::PModuleSymbol> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::PModuleSymbol> Definition; };
}

/******************************************************************************
** /syn/core/cpp_graph.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::PCppDefine> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::PCppDefine> Definition; };
}

/******************************************************************************
** /syn/core/containers.h
******************************************************************************/
namespace syn
{
    template<> struct type_define<::syn::core::Vector> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::Vector> Definition; };
    template<> struct type_define<::syn::core::ByteVector> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::ByteVector> Definition; };
    
    template<> struct type_define<::syn::core::Dictionary> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::Dictionary> Definition; };
    template<> struct type_define<::syn::core::StringDictionary> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::StringDictionary> Definition; };
    template<> struct type_define<::syn::core::SymbolDictionary> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::SymbolDictionary> Definition; };
    
    template<> struct type_define<::syn::core::Set> { CULTLANG_SYNDICATE_EXPORTED static syn::Define<::syn::core::Set> Definition; };
}

