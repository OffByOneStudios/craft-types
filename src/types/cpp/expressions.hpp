#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	template<typename T>
	struct VarArgs final
	{
		typedef typename T value_type;
		std::vector<T> args;
	};

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, instance>::value && !std::is_same<typename T::instance_type, void>::value >::type* = nullptr>
	IExpression* to_expression()
	{
		return new ExpressionConcrete(graph().recoverNode(cpptype<typename T::instance_type>::typeDesc().asNode()));
	}

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, instance>::value && std::is_same<typename T::instance_type, void>::value >::type* = nullptr>
		IExpression* to_expression()
	{
		return ExpressionSpecial::Any;
	}

	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, VarArgs>::value >::type* = nullptr>
		IExpression* to_expression()
	{
		return to_expression<typename T::value_type>();
	}

	template<typename T,
		typename std::enable_if< std::is_same<T, void>::value >::type* = nullptr>
	IExpression* to_expression()
	{
		return ExpressionSpecial::Void;
	}

	template<typename ...TArgs,
		typename std::enable_if< stdext::is_specialization<typename stdext::parampack_last<TArgs...>::type, VarArgs>::value >::type* = nullptr>
	IExpression* to_expression_tuple()
	{
		std::vector<IExpression*> vec = { to_expression<TArgs>()... };
		IExpression* collect = vec.back(); vec.pop_back();

		return new ExpressionTuple(vec, collect);
	}

	template<typename ...TArgs,
		typename std::enable_if< !stdext::is_specialization<typename stdext::parampack_last<TArgs...>::type, VarArgs>::value >::type* = nullptr>
		IExpression* to_expression_tuple()
	{
		return new ExpressionTuple({ to_expression<TArgs>()... });
	}

	template<typename TRet, typename ...TArgs>
	std::tuple<ExpressionStore, Function> to_expression_and_function(TRet (*fn)(TArgs...))
	{
		auto arrow = new ExpressionArrow(to_expression_tuple<TArgs...>(), to_expression<TRet>());

		return std::make_tuple(ExpressionStore(arrow), Function{ fn });
	}

	template<typename TClass, typename TRet, typename ...TArgs>
	std::tuple<ExpressionStore, Function> to_expression_and_function(TRet (TClass::*fn)(TArgs...))
	{
		
	}
}}