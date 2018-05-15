#pragma once
#include "types/common.h"
#include "types/core.h"

namespace craft {
namespace types
{
	template<typename T>
	struct VarArgs final
	{
		typedef T value_type;
		std::vector<T> args;
	};

	//
	// to_expression - simple
	//

	inline IExpression* to_expression(TypeId tid)
	{
		return new ExpressionConcrete(tid.node);
	}

	inline IExpression* to_expression_tuple(std::vector<TypeId> const& tids)
	{
		std::vector<IExpression*> exprs;
		exprs.reserve(tids.size());
		std::transform(tids.begin(), tids.end(), std::back_inserter(exprs),
			[](TypeId const& tid) { return to_expression(tid); });

		return new ExpressionTuple(exprs);
	}

	inline IExpression* to_expression_tuple(GenericInvoke const& invoke)
	{
		std::vector<IExpression*> exprs;
		exprs.reserve(invoke.args.size());
		std::transform(invoke.args.begin(), invoke.args.end(), std::back_inserter(exprs),
			[](instance<> inst) { return to_expression(inst.typeId()); });

		return new ExpressionTuple(exprs);
	}

	//
	// to_expression - templated
	//

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, instance>::value && !std::is_same<typename T::instance_type, void>::value >::type* = nullptr>
		inline IExpression* to_expression()
	{
		return new ExpressionConcrete(cpptype<typename T::instance_type>::typeDesc());
	}

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, instance>::value && std::is_same<typename T::instance_type, void>::value >::type* = nullptr>
	inline IExpression* to_expression()
	{
		return &ExpressionAny::Value;
	}

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, VarArgs>::value >::type* = nullptr>
	inline IExpression* to_expression()
	{
		return to_expression<typename T::value_type>();
	}

	template<typename T,
		typename std::enable_if< std::is_same<T, void>::value >::type* = nullptr>
	inline IExpression* to_expression()
	{
		return &ExpressionVoid::Value;
	}

	template<typename ...TArgs,
		typename std::enable_if< stdext::is_specialization<typename stdext::parampack_last<TArgs...>::type, VarArgs>::value >::type* = nullptr>
	inline IExpression* to_expression_tuple()
	{
		std::vector<IExpression*> vec = { to_expression<TArgs>()... };
		IExpression* collect = vec.back(); vec.pop_back();

		return new ExpressionTuple(vec, collect);
	}

	template<typename ...TArgs,
		typename std::enable_if< !stdext::is_specialization<typename stdext::parampack_last<TArgs...>::type, VarArgs>::value >::type* = nullptr>
	inline IExpression* to_expression_tuple()
	{
		return new ExpressionTuple({ to_expression<TArgs>()... });
	}

	template<typename ...TArgs,
		typename std::enable_if< sizeof...(TArgs) == 0 >::type* = nullptr>
	inline IExpression* to_expression_tuple()
	{
		return new ExpressionTuple({ to_expression<TArgs>()... });
	}

	template<typename TRet, typename ...TArgs>
	inline std::tuple<ExpressionStore, Function> to_expression_and_function(TRet (*fn)(TArgs...))
	{
		auto arrow = new ExpressionArrow(to_expression_tuple<TArgs...>(), to_expression<TRet>());

		return std::make_tuple(ExpressionStore(arrow), Function{ fn });
	}

	template<typename TClass, typename TRet, typename ...TArgs>
	inline std::tuple<ExpressionStore, Function> to_expression_and_function(TRet (TClass::*fn)(TArgs...))
	{
		
	}

	template<typename T,
		typename std::enable_if< !std::is_pointer<T>::value >::type* = nullptr>
		inline std::tuple<ExpressionStore, Function> to_expression_and_function(T fn)
	{
		return to_expression_and_function(+fn);
	}

	//
	// invoke
	//

	inline instance<> invoke(ExpressionStore const& exs, Function const& f, GenericInvoke const& i)
	{
		assert(exs.root()->kind() == cpptype<ExpressionArrow>::typeDesc().asId());
		auto arrow = (ExpressionArrow const*)exs.root();

		assert(arrow->input->kind() == cpptype<ExpressionTuple>::typeDesc().asId());
		auto tuple = (ExpressionTuple const*)arrow->input;

		auto t_size = tuple->entries.size();

		// varargs
		if (tuple->varType != nullptr)
		{
			assert(t_size <= i.args.size());

			// TODO: improve
			typedef VarArgs<instance<>> VarArgs;
			VarArgs va;
			va.args.reserve(i.args.size() - t_size);
			std::copy(i.args.begin() + t_size, i.args.end(), std::back_inserter(va.args));

			// WITH ret
			if (arrow->output != &ExpressionVoid::Value)
			{
				switch (t_size)
				{
				case 0:
					return reinterpret_cast<instance<>(*)(VarArgs)>(f._fn)(va);
				case 1:
					return reinterpret_cast<instance<>(*)(instance<>, VarArgs)>(f._fn)(i.args[0], va);
				case 2:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], va);
				case 3:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], va);
				case 4:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], va);
				case 5:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], va);
				case 6:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], i.args[5], va);
				default:
					throw type_runtime_error("invoke not compiled for this.");
				}
			}
			// withOUT ret
			else
			{
				switch (t_size)
				{
				case 0:
					reinterpret_cast<void(*)(VarArgs)>(f._fn)(va); break;
				case 1:
					reinterpret_cast<void(*)(instance<>, VarArgs)>(f._fn)(i.args[0], va); break;
				case 2:
					reinterpret_cast<void(*)(instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], va); break;
				case 3:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], va); break;
				case 4:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], va); break;
				case 5:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], va); break;
				case 6:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>, instance<>, instance<>, VarArgs)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], i.args[5], va); break;
				default:
					throw type_runtime_error("invoke not compiled for this.");
				}

				return instance<>();
			}
		}
		// standard
		else
		{
			assert(t_size == i.args.size());

			// WITH ret
			if (arrow->output != &ExpressionVoid::Value)
			{
				switch (t_size)
				{
				case 0:
					return reinterpret_cast<instance<>(*)()>(f._fn)();
				case 1:
					return reinterpret_cast<instance<>(*)(instance<>)>(f._fn)(i.args[0]);
				case 2:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>)>(f._fn)(i.args[0], i.args[1]);
				case 3:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2]);
				case 4:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3]);
				case 5:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4]);
				case 6:
					return reinterpret_cast<instance<>(*)(instance<>, instance<>, instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], i.args[5]);
				default:
					throw type_runtime_error("invoke not compiled for this.");
				}
			}
			// withOUT ret
			else
			{
				switch (t_size)
				{
				case 0:
					reinterpret_cast<void(*)()>(f._fn)(); break;
				case 1:
					reinterpret_cast<void(*)(instance<>)>(f._fn)(i.args[0]); break;
				case 2:
					reinterpret_cast<void(*)(instance<>, instance<>)>(f._fn)(i.args[0], i.args[1]); break;
				case 3:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2]); break;
				case 4:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3]); break;
				case 5:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4]); break;
				case 6:
					reinterpret_cast<void(*)(instance<>, instance<>, instance<>, instance<>, instance<>, instance<>)>(f._fn)(i.args[0], i.args[1], i.args[2], i.args[3], i.args[4], i.args[5]); break;
				default:
					throw type_runtime_error("invoke not compiled for this.");
				}

				return instance<>();
			}
		}

		throw stdext::exception("Malformed Invoke");
	}
}}