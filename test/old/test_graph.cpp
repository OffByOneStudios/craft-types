#include "types/common.h"
#include "basic_object.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;
using namespace craft::types;


go_bandit([]()
{
	/*
	Testing the properties of the graph system
	*/
	describe("craft::types::graph()", []()
	{
		it("graph().get(cpptype<_>::typeDesc()).ptr() == cpptype<_>::typeDesc().asNode()", [&]()
		{
			//AssertThat(graph().get(cpptype<TestBasicObject>::typeDesc()).ptr(), Equals(cpptype<TestBasicObject>::typeDesc().asNode()));
		});
	});
});
