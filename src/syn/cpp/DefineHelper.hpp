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

    template<typename TFinal>
    class DefineHelperLibraryBase
    {
    protected:
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

    public:

    };

    template<typename TFinal>
    class DefineHelperLibraryNaming
        : protected DefineHelperLibraryBase<TFinal>
    {
    public:
        inline DefineHelperLibraryNaming& name(std::string const& name)
        {
            auto sym = s().require(name);
            g().addProp<core::PModuleSymbol>({ sym }, node());
            return *this;
        }
        inline DefineHelperLibraryNaming& space(std::string const& name)
        {
            // TODO build namespace objects
            return *this;
        }
        inline DefineHelperLibraryNaming& module(Module const&)
        {
            auto sym = s().require(name);
            g().addProp<core::PModuleSymbol>({ sym }, node());
            return *this;
        }
    };

    template<typename TFinal, typename TDispatch, typename Enable=void>
    class DefineHelperPolicy
        : public DefineHelperLibraryNaming<TFinal>
    {
        static_assert("No policy for that type pattern, try using Define<void> instead.");
    };

    template<typename TFinal>
    class DefineHelperPolicy<TFinal, void, void> // TODO why is it dispatching here
        : public DefineHelperLibraryNaming<TFinal>
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
