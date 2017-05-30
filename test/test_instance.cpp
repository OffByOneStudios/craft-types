#include "common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

go_bandit([]()
{
	/*
		Testing the properties of a generic instance class.
	*/
	describe("`craft::instance<>`", []()
	{
		instance<> inst;

		describe("when null,", [&]()
		{
			it("isNull is true", [&]()
			{
				AssertThat(inst.isNull(), IsTrue());
			});
			it("bool conversion is false", [&]()
			{
				AssertThat(static_cast<bool>(inst), IsFalse());
			});
			it("get is nullptr", [&]()
			{
				AssertThat(inst.get(), IsNull());
			});
			it("typeId is None", [&]()
			{
				AssertThat(inst.typeId(), Equals(types::None));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<void>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<void>, inst));
		});

		describe("with std::string value \"test\",", [&]()
		{
			before_each([&]() {
				inst = instance<std::string>::make("test");
			});

			it("isNull is false", [&]()
			{
				AssertThat(inst.isNull(), IsFalse());
			});
			it("bool conversion is true", [&]()
			{
				AssertThat(static_cast<bool>(inst), IsTrue());
			});
			it("get is not nullptr", [&]()
			{
				AssertThat(inst.get(), !IsNull());
			});
			it(".typeId() is type<std::string>::typeId()", [&]()
			{
				AssertThat(inst.typeId(), Equals(type<std::string>::typeId()));
			});
			it("*(std::string*) .get() is \"test\"", [&]()
			{
				AssertThat(*(std::string*)inst.get(), Equals("test"));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<void>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<void>, inst));
		});
	});
});
