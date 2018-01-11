#include "common.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

go_bandit([]()
{
	/*
	Testing the properties of the graph system
	*/
	describe("craft::types::TypeGraphSystem", []()
	{
		types::TypeGraphSystem* graph_system;

		before_each([&]() {
			graph_system = new types::TypeGraphSystem();
		});
		after_each([&]() {
			delete graph_system;
		});


	});
});