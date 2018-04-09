#include "simple_multimethod.h"
#include "basic_object.h"
#include "adv_object.h"

using namespace craft;
using namespace craft::types;

uintptr_t test_method_simple(TestBasicObject const* this_, TestAdvancedObject* that)
{
	return this_->sameAs_string(that->a_concrete_instance.get());
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_simple)
{
	//_.add_method(&TestBasicObject::sameAs_string); //Not supported 
	_.add_method(test_method_simple);
	_.add_method_lambdaHack<uintptr_t(*)(void*, TestAdvancedObject const*)>(
		[](auto a, auto b) -> uintptr_t
		{
			return a == b->a_generic_instance.get();
		});
}

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
