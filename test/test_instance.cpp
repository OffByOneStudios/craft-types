#include "common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

go_bandit([]()
{
	/*
	First we test the properties of a classic std wrapped class (std::string)
	*/
	describe("`craft::instance<std::string>`", []()
	{
		describe("constructors,", [&]()
		{
			it("via stack", [&]()
			{
				instance<std::string> _;
				AssertThat(_.isNull(), IsTrue());
			});
			it("via nullptr", [&]()
			{
				instance<std::string> _ = instance<std::string>(nullptr);
				AssertThat(_.isNull(), IsTrue());
			});
			it("make", [&]()
			{
				instance<std::string> _ = instance<std::string>::make();
				AssertThat(_.isNull(), IsFalse());
			});
			it("via new", [&]()
			{
				instance<std::string> _ = instance<std::string>(new std::string());
				AssertThat(_.isNull(), IsFalse());
			});
		});

		instance<std::string> inst;

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
			it("typeId is type<std::string>::typeId()", [&]()
			{
				AssertThat(inst.typeId(), Equals(type<std::string>::typeId()));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<std::string>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<std::string>, inst));
		});

		describe("with value \"test\",", [&]()
		{
			before_each([&]() {
				inst = instance<std::string>::make("test");
			});

			it(".isNull() is false", [&]()
			{
				AssertThat(inst.isNull(), IsFalse());
			});
			it(".operator bool() is true", [&]()
			{
				AssertThat(static_cast<bool>(inst), IsTrue());
			});
			it(".get() is not nullptr", [&]()
			{
				AssertThat(inst.get(), !IsNull());
			});
			it(".typeId() is type<std::string>::typeId()", [&]()
			{
				AssertThat(inst.typeId(), Equals(type<std::string>::typeId()));
			});
			it("* .get() is \"test\"", [&]()
			{
				AssertThat(*inst.get(), Equals("test"));
			});
			it(".operator *() is \"test\"", [&]()
			{
				AssertThat(*inst, Equals("test"));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<std::string>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<std::string>, inst));
		});

		instance<std::string> inst_alias, inst_same, inst_diff;

		describe("with value \"test\" and others,", [&]()
		{
			before_each([&]() {
				inst_alias = inst = instance<std::string>::make("test");
				inst_same = instance<std::string>::make("test");
				inst_diff = instance<std::string>::make("diff");
			});
			it("== alias", [&]()
			{
				AssertThat(inst == inst_alias, IsTrue());
			});
			it("== same", [&]()
			{
				AssertThat(inst == inst_same, IsFalse());
			});
			it("== diff", [&]()
			{
				AssertThat(inst == inst_same, IsFalse());
			});
			it(".operator ->() == .get()", [&]()
			{
				AssertThat(inst.operator ->() == inst.get(), IsTrue());
			});
		});
	});

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
			it("typeId is 0", [&]()
			{
				AssertThat(inst.typeId(), Equals(0));
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
