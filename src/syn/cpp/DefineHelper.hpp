#pragma once
#include "syn/syn.h"

namespace syn {
namespace details
{
    struct define_error
        : stdext::exception
    {
        using exception::exception;
    };

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
            auto n = actual().define->node;
            if (n == nullptr)
                throw define_error("node not pre-created...");
            return n;
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

    template<typename TFinal>
    class DefineHelperLibraryTyping
        : protected DefineHelperLibraryBase<TFinal, DefineHelperLibraryTyping<TFinal>>
    {
	protected:
        typedef DefineHelperLibraryBase<TFinal, DefineHelperLibraryTyping<TFinal>> Base;
		using Base::g;
		using Base::s;
		using Base::node;

    public:
        inline void subtypes(syn::Abstract const& abstract_)
        {
            g().template addEdge<core::EIsA>({ }, { node(), abstract_.node });
        }
    };

    namespace _details
    {
        template<typename TClassType, typename TReturn = void, typename TArgs = void> struct _t_register_class_function;

        template<typename TClassType, typename TReturn, typename TThat, typename... TArgs>
        struct _t_register_class_function <TClassType, TReturn, std::tuple<TThat, TArgs...>>
        {
			typedef TReturn ( TClassType::* TPFunc ) ( TArgs... );
            template<auto PFunc>
			static inline TReturn _trampoline(TClassType* ttype, TArgs... args)
			{
				return (ttype->*PFunc)( std::forward<TArgs>(args)... );
			}

            template<auto PFunc, typename TDefine>
            static inline void _exec(TDefine* define)
            {
				void (*fnptr)() = reinterpret_cast<void (*)()>( (TReturn (*)(TClassType*, TArgs...))&_trampoline<PFunc> );
                auto n = define->g().template addNode<core::NFunction>({ fnptr });
            }
        };
        template<typename TClassType>
        struct _t_register_class_function <TClassType, void, void>
        {
            template<auto PFunc, typename TDefine>
            static inline void _exec(TDefine* define)
            {
                return _t_register_class_function<
                        TClassType,
                        boost::callable_traits::return_type_t<decltype(PFunc)>,
                        boost::callable_traits::args_t<decltype(PFunc)>
                    >::template _exec<PFunc, TDefine>(define);
            }
        };
    }

    template<typename TFinal, typename TType>
    class DefineHelperLibraryStructure
        : protected DefineHelperLibraryBase<TFinal, DefineHelperLibraryStructure<TFinal, TType>>
    {
	protected:
        typedef DefineHelperLibraryBase<TFinal, DefineHelperLibraryStructure<TFinal, TType>> Base;
		using Base::g;
		using Base::s;
		using Base::node;

		template<typename TClassType, typename TReturn, typename TArgs> friend struct _details::_t_register_class_function;

    protected:
        template<typename TOtherType>
        inline constexpr ptrdiff_t _inherits_offset()
        {
            auto original_pointer = uintptr_t(0x10000);
            auto casted_pointer = reinterpret_cast<uintptr_t>((TOtherType*)reinterpret_cast<TType*>(original_pointer));
            return original_pointer - casted_pointer;
        }

    public:
        template<typename TMemberType>
        inline typename std::enable_if<true,
            void>::type member(std::string const& name, TMemberType TType::* memptr)
        {

        }

        template<auto PMethod>
        inline typename std::enable_if<true,
            void>::type method(std::string const& name)
        {
            _details::_t_register_class_function<TType>::template _exec<PMethod>(this);
        }
        template<auto PMethod, typename TMulti>
        inline typename std::enable_if<TMulti::kind == CppDefineKind::Dispatcher,
            void>::type method(TMulti& method)
        {
            _details::_t_register_class_function<TType>::template _exec<PMethod>(this);
        }
        
        template<typename FMethod>
        inline typename std::enable_if<true,
            void>::type method(std::string const& name, FMethod methptr)
        {
            
        }
        template<typename TMulti, typename FMethod>
        inline typename std::enable_if<TMulti::kind == CppDefineKind::Dispatcher,
            void>::type method(TMulti& method, FMethod methptr)
        {
            
        }

    public:
        template<typename TOtherType>
        inline typename std::enable_if<syn::type<TOtherType>::kind == CppDefineKind::Struct,
            void>::type inherits()
        {
            auto e = g().template addEdge<core::EIsA>({ }, { node(), syn::type<TOtherType>::graphNode() });
            g().template addProp<core::PCompositionalCast>({ _inherits_offset<TOtherType>() }, e);
        }

        template<typename TOtherType>
        inline typename std::enable_if<syn::type<TOtherType>::kind == CppDefineKind::Struct,
            void>::type implements()
        {
            auto e = g().template addEdge<core::EIsA>({ }, { node(), syn::type<TOtherType>::graphNode() });
            //g().template addProp<core::PCompositionalCast>({ _inherits_offset<TOtherType>() }, e);
        }

    public:
        template<typename FConstructor>
        inline void constructor(FConstructor constructor)
        {

        }

        template<typename... TArgs>
        inline void constructor()
        {

        }

        inline void destructor()
        {

        }
        
        template<typename TOtherType>
        inline void assigner()
        {

        }

    public:
        inline void detectLifecycle()
        {
            if constexpr (std::is_default_constructible_v<TType>) constructor();
            if constexpr (std::is_destructible_v<TType>) destructor();

            if constexpr (std::is_copy_constructible_v<TType>) constructor<TType const&>();
            if constexpr (std::is_move_constructible_v<TType>) constructor<TType &&>();
            if constexpr (std::is_copy_assignable_v<TType>) assigner<TType const&>();
            if constexpr (std::is_move_constructible_v<TType>) assigner<TType &&>();



        }

        inline void plainOldData()
        {
            
        }
    };

    template<typename TFinal>
    class DefineHelperLibraryMultimethod
        : protected DefineHelperLibraryBase<TFinal, DefineHelperLibraryMultimethod<TFinal>>
    {
	protected:
        typedef DefineHelperLibraryBase<TFinal, DefineHelperLibraryMultimethod<TFinal>> Base;
		using Base::g;
		using Base::s;
		using Base::node;

    public:
        template<typename TMethodType>
        inline typename std::enable_if<true,
            void>::type method(TMethodType methptr)
        {
            
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
        std::enable_if_t<
            TType::kind == CppDefineKind::Abstract>>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, TType, void>>
        , public DefineHelperLibraryNaming<TFinal>
        , public DefineHelperLibraryTyping<TFinal>
    {

    };

    template<typename TFinal, typename TType>
    class DefineHelperPolicy<TFinal, TType,
        std::enable_if_t<
            TType::kind == CppDefineKind::Struct
            && !std::is_class<typename TType::Type>::value>>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, TType, void>>
        , public DefineHelperLibraryNaming<TFinal>
        , public DefineHelperLibraryTyping<TFinal>
    {

    };

    template<typename TFinal, typename TType>
    class DefineHelperPolicy<TFinal, TType,
        std::enable_if_t<
            TType::kind == CppDefineKind::Struct
            && std::is_class<typename TType::Type>::value>>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, TType, void>>
        , public DefineHelperLibraryNaming<TFinal>
        , public DefineHelperLibraryTyping<TFinal>
        , public DefineHelperLibraryStructure<TFinal, typename TType::Type>
    {

    };

    template<typename TFinal, typename TType>
    class DefineHelperPolicy<TFinal, TType,
        std::enable_if_t<TType::kind == CppDefineKind::Dispatcher>>
        : public DefineHelperLibraryBase<TFinal, DefineHelperPolicy<TFinal, TType, void>>
        , public DefineHelperLibraryNaming<TFinal>
        , public DefineHelperLibraryMultimethod<TFinal>
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
