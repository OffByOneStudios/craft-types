#include "common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

go_bandit([]()
{
	describe("`craft::instance<>`", []()
	{
		instance<> inst;

		it("starts null", [&]()
		{
			AssertThat(inst.isNull(), IsTrue());
		});

		describe("when null,", [&]()
		{
			it("bool conversion is false", [&]()
			{
				AssertThat(static_cast<bool>(inst), IsFalse());
			});
			it("typeId is 0", [&]()
			{
				AssertThat(inst.typeId(), Equals(0));
			});
			it("provider hasFeature false", [&]()
			{
				AssertThat(inst.hasFeature<PIdentifier>(), IsFalse());
			});
			it("provider getFeature nullptr", [&]()
			{
				AssertThat(inst.getFeature<PIdentifier>(), IsNull());
			});
			it("aspect hasFeature false", [&]()
			{
				AssertThat(inst.hasFeature<SLifecycle>(), IsFalse());
			});
			it("aspect getFeature nullptr", [&]()
			{
				AssertThat(inst.getFeature<SLifecycle>(), IsNull());
			});
		});
	});

	describe("`craft::instance<std::string>`", []()
	{
		instance<std::string> inst;

		it("starts null", [&]()
		{
			AssertThat(inst.isNull(), IsTrue());
		});

		describe("when null,", [&]()
		{
			it("bool conversion is false", [&]()
			{
				AssertThat(static_cast<bool>(inst), IsFalse());
			});
			it("typeId is 0", [&]()
			{
				AssertThat(inst.typeId(), Equals(0));
			});
			it("hasFeature false", [&]()
			{
				AssertThat(inst.hasFeature<PIdentifier>(), IsFalse());
			});
			it("getFeature nullptr", [&]()
			{
				AssertThat(inst.getFeature<PIdentifier>(), IsNull());
			});
		});
	});
});
