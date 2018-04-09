#pragma once
#include "../common.h"
#include "../core.h"

namespace craft {
namespace types
{
	template<typename T,
		typename std::enable_if< stdext::is_specialization<T, instance>::value >::type* = nullptr>
	IExpression* to_expression()
	{
		return new ExpressionConcrete(cpptype<typename T::instance_type>::typeDesc().asNode());
	}

	template<typename ...TArgs>
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