#include "simple_multimethod.h"
#include "basic_object.h"
#include "adv_object.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;
using namespace craft::types;

uintptr_t test_method_simple(TestBasicObject const* this_, TestBasicObject* that)
{
	return this_->sameAs_string(that);
}

uintptr_t test_method_simple(TestBasicObject* this_, TestAdvancedObject* that)
{
	return this_->sameAs_string(that->a_concrete_instance.get());
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_simple)
{
	//_.add_method(&TestBasicObject::sameAs_string); //Not supported 
	_.add_method((uintptr_t (*)(TestBasicObject const* this_, TestBasicObject* that))test_method_simple);
	_.add_method((uintptr_t (*)(TestBasicObject* this_, TestAdvancedObject* that))test_method_simple);
	_.add_method(
		[](void* a, TestAdvancedObject const* b) -> uintptr_t
		{
			return a == b->a_generic_instance.get();
		});
	_.add_method(
		[](std::string* s) -> TestBasicObject*
		{
			auto tbo = new TestBasicObject();
			tbo->a_string = *s;
			return tbo;
		});
}

/******************************************************************************
** Tests
******************************************************************************/

go_bandit([]()
{
	/*
	Testing the properties of the graph system
	*/
	describe("MultiMethod SimpleDispatcher", []()
	{
		it("test_method_simple dispatches (TestBasicObject*, TestBasicObject*) -> true", [&]()
		{
			auto tbo = new TestBasicObject();
			tbo->a_string = "hello";

			auto res = test_multimethod_simple(tbo, tbo);

			AssertThat(std::get<0>(res), Equals(SimpleDispatcher::Any));
			AssertThat((bool)(uintptr_t)std::get<1>(res), Equals(true));
		});
		
		it("test_method_simple dispatches (TestBasicObject*, TestBasicObject*) -> false", [&]()
		{
			auto tbo0 = new TestBasicObject();
			tbo0->a_string = "hello";
			auto tbo1 = new TestBasicObject();
			tbo1->a_string = "gootbye";

			auto res = test_multimethod_simple(tbo0, tbo1);

			AssertThat(std::get<0>(res), Equals(SimpleDispatcher::Any));
			AssertThat((bool)(uintptr_t)std::get<1>(res), Equals(false));
		});

		it("test_method_simple dispatches (std::string*) -> <TestBasicObject*>", [&]()
		{
			std::string hello("hello");

			auto res = test_multimethod_simple(&hello);

			AssertThat(std::get<0>(res), Equals(cpptype<TestBasicObject>::typeDesc().asId().node));
			AssertThat(((TestBasicObject*)std::get<1>(res))->a_string, Equals("hello"));
		});
	});
});
