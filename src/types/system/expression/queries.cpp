#include "types/common.h"
#include "types/core.h"
#include "types/system/expression/queries.h"
#include "types/boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

CRAFT_MULTIMETHOD_DEFINE(craft::types::isSubtypeMethod)
{
	_.add_method([](IExpression* left, ExpressionAny* right) { return true; });
	_.add_method([](ExpressionAny* left, IExpression* right) { return false; });
	_.add_method([](ExpressionBottom* left, IExpression* right) { return true; });
	_.add_method([](IExpression* left, ExpressionBottom* right) { return false; });

	_.add_method([](ExpressionConcrete* left, ExpressionConcrete* right)
	{
		if (left->node.ptr() == right->node.ptr())
			return true;

		// Type System

		return false;
	});

	_.add_method([](ExpressionTuple* left, ExpressionTuple* right)
	{
		auto var = right->varType != nullptr;
		auto left_count = left->entries.size();
		auto right_count = right->entries.size();

		if (var)
		{
			if (left->varType == nullptr)
			{
				// If the left is not a var args it must be greater and equal to
				if (left_count < right_count) return false;
			}
			else
			{
				// Might as well check the var args now
				if (!isSubtype(left->varType, right->varType)) return false;

				if (left_count < right_count)
				{
					// Check the left hand side's var args responsibilities
					for (auto i = left_count; i < right_count; ++i)
					{
						if (!isSubtype(left->varType, right->entries[i]))
							return false;
					}
				}
			}
		}
		else
		{
			// With no var args the counts must match...
			if (left_count != right_count) return false;

			// ... and the left must not be var args
			if (left->varType != nullptr) return false;
		}

		// Time to do the core match
		for (auto i = 0; i < left_count; ++i)
		{
			auto right_i = (i >= right_count) ? right->varType : right->entries[i];
			if (!isSubtype(left->entries[i], right_i))
				return false;
		}

		return true;
	});

	_.add_method([](ExpressionArrow* left, ExpressionArrow* right)
	{
		return isSubtype(left->input, right->input) && isSubtype(left->output, right->output);
	});

	_.add_method([](void* left, void* right) { return false; });
}

bool craft::types::isSubtype(IExpression* left, IExpression* right)
{
	auto res = isSubtypeMethod.invoke(SimpleDispatcher::Invoke{ left->invokeArg(), right->invokeArg() });

	return ((uintptr_t)std::get<1>(res)) != 0;
}
