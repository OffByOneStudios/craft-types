#pragma once
#include "syn/syn.h"

namespace syn {
namespace details
{
    class DefineHelperActual
    {
    public:
        CppDefine* define;

        inline DefineHelperActual(CppDefine* define)
            : define(define)
        { }
    };

    template<typename TFinal, typename TLibrary>
    class DefineHelperLibraryBase
    {
    public:
        inline DefineHelperActual& actual()
        {
            return *(DefineHelperActual*)(TFinal*)this;
        }

        inline SymbolTable& s()
        {
            return global_store().s();
        }

        inline Graph& g()
        {
            return global_store().g();
        }

        inline Graph::Node* node()
        {
            return actual().define->node;
        }

    };

    template<typename TFinal>
    class DefineHelperLibraryNaming
        : protected DefineHelperLibraryBase<TFinal, DefineHelperLibraryNaming<TFinal>>
    {
	protected:
        typedef DefineHelperLibraryBase<TFinal, DefineHelperLibraryNaming<TFinal>> Base;
		using Base::g;
		using Base::s;
		using Base::node;

    public:
        inline DefineHelperLibraryNaming& name(std::string const& name)
        {
            auto sym = s().require(name);
            g().template addProp<core::PModuleSymbol>({ sym }, node());
            return *this;
        }
        inline DefineHelperLibraryNaming& space(std::string const& name)
        {
            // TODO build namespace objects
            return *this;
        }
        inline DefineHelperLibraryNaming& module(Module const& module)
        {
			
            return *this;
        }
    };

    template<typename TFinal, typename TType>
    class DefineHelperLibraryStructure
        : protected DefineHelperLibraryBase<TFinal, DefineHelperLibraryStructure<TFinal, TType>>
    {
	protected:
        typedef DefineHelperLibraryBase<TFinal, DefineHelperLibraryStructure<TFinal, TType>> Base;
		using Base::g;
		using Base::s;
		using Base::node;

    protected:
        template<typename TOtherType>
        inline constexpr ptrdiff_t _inherits_offset()
        {
            auto original_pointer = uintptr_t(0x10000);
            auto casted_pointer = reinterpret_cast<uintptr_t>((TOtherType*)reinterpret_cast<TType*>(original_pointer));
            return original_pointer - casted_pointer;
        }

    public:
        template<typename TOtherType>
        inline typename std::enable_if<syn::type<TOtherType>::kind == CppDefineKind::Struct,
            void>::type inherits()
        {
            auto e = g().template addEdge<core::EIsA>({ }, { node(), syn::type<TOtherType>::graphNode() });
            g().template addProp<core::PCompositionalCast>({ _inherits_offset<TOtherType>() }, e);
        }
    };

    template<typename TFinal, typename TDispatch, typename Enable=void>
    class DefineHelperPolicy
    {
        static_assert("No policy for that type pattern, try using Define<void> instead.");
    };

    template<typename TFinal>
    class DefineHelperPolicy<TFinal, void, void>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, void, void>>
    {

    };

    template<typename TFinal, typename TType>
    class DefineHelperPolicy<TFinal, TType,
        std::enable_if_t<TType::kind == CppDefineKind::Struct>>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, TType, void>>
        , public DefineHelperLibraryNaming<TFinal>
        , public DefineHelperLibraryStructure<TFinal, typename TType::Type>
    {

    };

    template<typename T>
    class DefineHelper
        : public DefineHelperActual
        , public DefineHelperPolicy<DefineHelper<T>, T>
    {
    public:
        using DefineHelperActual::DefineHelperActual;
    };
}}
