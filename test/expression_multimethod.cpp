#include "expression_multimethod.h"
#include "basic_object.h"
#include "adv_object.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;
using namespace craft::types;

instance<bool> test_method_expression(instance<TestBasicObject> this_, instance<TestBasicObject> that)
{
	return instance<bool>::make(this_->sameAs_string(that.get()));
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_expression)
{
	_.add_method(test_method_expression);
	_.add_method([](instance<std::string> s)
	{
		auto res = instance<TestBasicObject>::make();
		res->a_string = *s;
		return res;
	});
}

CRAFT_MULTIMETHOD_DEFINE(test_multimethod_instance)
{

}


/******************************************************************************
** Tests
******************************************************************************/

go_bandit([]()
{
	/*
	Testing the properties of the graph system
	*/
	describe("MultiMethod ExpressionDispatcher", []()
	{
		it("test_multimethod_expression dispatches (TestBasicObject*, TestBasicObject*) -> true", [&]()
		{
			auto tbo = instance<TestBasicObject>::make();
			tbo->a_string = "hello";

			auto res = test_multimethod_expression(tbo, tbo);

			AssertThat(res.typeId(), Equals(cpptype<bool>::typeDesc().asId()));
			AssertThat(*res.asType<bool>(), Equals(true));
		});

		it("test_multimethod_expression dispatches (TestBasicObject*, TestBasicObject*) -> false", [&]()
		{
			auto tbo0 = instance<TestBasicObject>::make();
			tbo0->a_string = "hello";
			auto tbo1 = instance<TestBasicObject>::make();
			tbo1->a_string = "gootbye";

			auto res = test_multimethod_expression(tbo0, tbo1);

			AssertThat(res.typeId(), Equals(cpptype<bool>::typeDesc().asId()));
			AssertThat(*res.asType<bool>(), Equals(false));
		});

		it("test_multimethod_expression dispatches (std::string*) -> <TestBasicObject*>", [&]()
		{
			auto hello = instance<std::string>::make("hello");

			auto res = test_multimethod_expression(hello);

			AssertThat(res.typeId(), Equals(cpptype<TestBasicObject>::typeDesc().asId()));
			AssertThat(res.asType<TestBasicObject>()->a_string, Equals("hello"));
		});
	});
});
