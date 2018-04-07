#include "../../common.h"
#include "../../core.h"
#include "queries.h"
#include "../../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

CRAFT_MULTIMETHOD_DEFINE(craft::types::isSubtypeMethod)
{

}

bool craft::types::isSubtype(IExpression* left, IExpression* right)
{
	auto res = isSubtypeMethod.invoke(SimpleDispatcher::Invoke{ left->invokeArg(), right->invokeArg() });

	return ((uintptr_t)std::get<1>(res)) != 0;
}
