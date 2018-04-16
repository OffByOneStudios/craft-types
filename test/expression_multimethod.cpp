#include "expression_multimethod.h"
#include "basic_object.h"
#include "adv_object.h"

using namespace craft;
using namespace craft::types;

instance<bool> test_method_expression(instance<TestBasicObject> this_, instance<TestAdvancedObject> that)
{
	return instance<bool>::make(this_->sameAs_string(that->a_concrete_instance.get()));
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_expression)
{
	_.add_method(test_method_expression);
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_instance)
{

}
