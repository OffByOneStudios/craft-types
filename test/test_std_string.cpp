#include "common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;
using namespace craft::types;

go_bandit([]()
{
	/*
		Test the properties of a classic std wrapped class (std::string)
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
				AssertThat(inst.typeId(), Equals(types::cpptype<std::string>::typeDesc().asId()));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<std::string>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<std::string>, inst));
		});

		describe("`<Provider: types.parse>`", [&]()
		{
			PParse* parser;

			before_each([&]() {
				parser = inst.getFeature<PParse>();
			});

			it(".hasFeature<PParse>() is true", [&]()
			{
				AssertThat(inst.hasFeature<PParse>(), IsTrue());
			});
			it(".getFeature<PParse>() is not nullptr", [&]()
			{
				AssertThat(inst.getFeature<PParse>(), !IsNull());
			});

			it(".parse(\"test\") is not null", [&]()
			{
				AssertThat(parser->parse("test").isNull(), IsFalse());
			});
			it(".parse(\"test\") is type<std::string>::typeId()", [&]()
			{
				AssertThat(parser->parse("test").typeId(), Equals(types::cpptype<std::string>::typeDesc().asId()));
			});
			it(".parse(\"test\") is \"test\"", [&]()
			{
				AssertThat(parser->parse("test").asType<std::string>().value(), Equals("test"));
			});
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
				AssertThat(inst.typeId(), Equals(types::cpptype<std::string>::typeDesc().asId()));
			});
			it(".value() is \"test\"", [&]()
			{
				AssertThat(inst.value(), Equals("test"));
			});
			it(".operator *() is \"test\"", [&]()
			{
				AssertThat(*inst, Equals("test"));
			});

			describe("empty providers,", std::bind(describe_no_empty_providers<std::string>, inst));
			describe("empty aspects,", std::bind(describe_no_empty_aspects<std::string>, inst));

			describe("`<Provider: types.str>`", [&]()
			{
				instance<PStringer> stringer;

				before_each([&]() {
					stringer = inst.asFeature<PStringer>();
				});

				it(".hasFeature<PStringer>() is true", [&]()
				{
					AssertThat(inst.hasFeature<PStringer>(), IsTrue());
				});
				it(".getFeature<PStringer>() is not nullptr", [&]()
				{
					AssertThat(inst.getFeature<PStringer>(), !IsNull());
				});

				it(".toString(_) is \"test\"", [&]()
				{
					AssertThat(stringer->toString(inst), Equals("test"));
				});
			});
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
});
