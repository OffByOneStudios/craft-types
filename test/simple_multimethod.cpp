#include "simple_multimethod.h"
#include "basic_object.h"
#include "adv_object.h"

bool test_method(TestBasicObject const* this_, TestAdvancedObject* that)
{
	return this_->sameAs_string(that->a_concrete_instance.get());
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_simple)
{
	_.add_method(&TestBasicObject::sameAs_string);
	_.add_method(test_method);
	_.add_method([](void* a, TestAdvancedObject const* b)
	{
		return a == b->a_generic_instance.get();
	});
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_expression)
{
	
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_instance)
{

}
