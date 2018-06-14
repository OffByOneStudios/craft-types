#include "common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

/*
	Testing the properties of PObjectContextual using a mock object.
*/

class MockObjectContextual
	: public virtual craft::types::Object
{
	CRAFT_OBJECT_DECLARE(MockObjectContextual);

	void contextualize(std::shared_ptr<craft::types::Context> c);

public:
	std::function<void (std::shared_ptr<craft::types::Context>)> contextualizer;
};

CRAFT_DEFINE(MockObjectContextual)
{
	_.use<craft::types::PObjectContextual>().byConfiguring<craft::types::ObjectContextualWrapper>()
		->withContextualizer(&MockObjectContextual::contextualize);

	_.defaults();
}

void MockObjectContextual::contextualize(std::shared_ptr<craft::types::Context> c)
{
	contextualizer(c);
}

/*
	Object implementing an interface for testing contextualize
*/

class StupidObjectContextual
	: public virtual craft::types::Object
	, public craft::types::Implements<SEmptyTestInstance>
{
	CRAFT_OBJECT_DECLARE(StupidObjectContextual);

public:
};

CRAFT_DEFINE(StupidObjectContextual)
{
	_.use<SEmptyTestInstance>().byCasting();

	_.defaults();
}

/*
	ACTUAL TESTS
*/

using namespace bandit;
using namespace snowhouse;

using namespace craft;

/*
void FeaturesContainer::contextualize(std::shared_ptr<types::Context> c)
{
	// TODO(OffByOneStudios/craft-types#1)
	_context = std::make_shared<Context>(c, this);
	_context = c;

	for (auto inst : _instances)
	{
		_context->promoteOnType(inst.typeId(), inst);
	}

	// TODO(OffByOneStudios/craft-types#1)
	_context->finalize();

	for (auto inst : _instances)
	{
		if (inst.hasFeature<types::PObjectContextual>())
		{
			inst.getFeature<types::PObjectContextual>()->contextualize(inst, _context);
		}
	}
}

void CommandManager::contextualize(std::shared_ptr<Context> c)
{
	_selection = c->by<SelectionManager>()->prime();

	auto ecs = c->by<Ecs>()->prime().asType<Ecs>();
	_e_orders = ecs->_ecs->require<COrder>();
	_e_commandQueue = ecs->_ecs->require<CCommandQueue>();

	_commands.clear();
	_commands.push_back({});
	_commands_byName[""] = 0;

	auto commandCollectors = c->by<PCommandCollector>()->objects();
	for (auto collector : commandCollectors)
	{
		addCollector(collector);
	}
}
*/



go_bandit([]()
{
	/*
	Testing the properties of the Context object
	*/
	describe("`craft::types::Context`", []()
	{
		describe("test Context", [&]() {
			std::shared_ptr<types::Context> ctx;

			before_each([&]() {
				ctx = std::make_shared<types::Context>(nullptr);
			});

			it("add and retrieve an object by type", [&]()
			{
				types::instance<> inst = types::instance<StupidObjectContextual>::make();
				ctx->promoteOnType(inst.typeId(), inst);

				AssertThat(ctx->byType(types::cpptype<StupidObjectContextual>::typeDesc())->prime().get(), Equals(inst.get()));
			});

			it("add and template-retrieve an object by type", [&]()
			{
				types::instance<> inst = types::instance<StupidObjectContextual>::make();
				ctx->promoteOnType(inst.typeId(), inst);

				AssertThat(ctx->by<StupidObjectContextual>()->prime().get(), Equals(inst.get()));
			});

			it("template-add and retrieve an object by type", [&]()
			{
				auto inst = types::instance<StupidObjectContextual>::make();
				ctx->promote(inst);

				AssertThat(ctx->byType(types::cpptype<StupidObjectContextual>::typeDesc())->prime().get(), Equals(inst.get()));
			});

			/*
			it("add and retrieve an object by feature", [&]()
			{
				types::instance<> inst = types::instance<StupidObjectContextual>::make();
				ctx->promoteOnFeature(SEmptyTestInstance::craft_s_typeDesc(), inst);

				AssertThat(ctx->byFeature(SEmptyTestInstance::craft_s_featureId())->prime().get(), Equals(inst.get()));
			});

			it("add and template-retrieve an object by feature", [&]()
			{
				types::instance<> inst = types::instance<StupidObjectContextual>::make();
				ctx->promoteOnFeature(SEmptyTestInstance::craft_s_typeDesc(), inst);

				AssertThat(ctx->by<SEmptyTestInstance>()->prime().get(), Equals(inst.get()));
			});

			it("template-add and retrieve an object by feature", [&]()
			{
				auto inst = types::instance<StupidObjectContextual>::make().asFeature<SEmptyTestInstance>();
				ctx->promote<SEmptyTestInstance>(inst);

				AssertThat(ctx->byFeature(SEmptyTestInstance::craft_s_featureId())->prime().get(), Equals(inst.get()));
			});
			*/
		});
	});

	/*
	Testing the properties of the Context object
	*/
	describe("`craft::types::PObjectContextual`", []()
	{

	});
});
