#include "types/common.h"

#include "empty_aspects.h"
#include "empty_providers.h"
#include "basic_object.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

go_bandit([]()
{
	/*
		Testing the properties of a generic instance class.
	*/
	describe("instance<> lifecycle management:", []()
	{
		instance<> inst;

		it("incref is safe", [&]()
		{
			inst.incref();
		});
		it("decref is safe", [&]()
		{
			inst.decref();
		});

		describe("ref counting", [&]()
		{
			before_each([&]()
			{
				inst = instance<std::string>::make();
			});

			it("one reference", [&]()
			{
				AssertThat(inst.refCount(), Equals(1));
			});
			it("manual reference management", [&]()
			{
				inst.incref(); inst.incref(); inst.incref();

				AssertThat(inst.refCount(), Equals(4));

				inst.decref(); inst.decref(); inst.decref();
			});
			it("assignment doesn't leak", [&]()
			{
				{
					auto i = inst;
					AssertThat(inst.refCount(), Equals(2));
					AssertThat(i.refCount(), Equals(2));
				}
				AssertThat(inst.refCount(), Equals(1));
			});
			it("copy construct doesn't leak", [&]()
			{
				{
					auto i(inst);
					AssertThat(inst.refCount(), Equals(2));
					AssertThat(i.refCount(), Equals(2));
				}
				AssertThat(inst.refCount(), Equals(1));
			});
			it("move assignment doesn't leak", [&]()
			{
				// TODO pause finalizer, use manual pointers, check refcount is 0 (after)
				{
					auto i = std::move(inst);
					AssertThat(i.refCount(), Equals(1));
				}
			});
			it("move construct doesn't leak", [&]()
			{
				// TODO pause finalizer, use manual pointers, check refcount is 0 (after)
				{
					auto i(std::move(inst));
					AssertThat(i.refCount(), Equals(1));
				}
			});
			it("manual pointers", [&]()
			{
				AssertThat(inst.refCount(), Equals(1));

				auto p = inst.asInternalPointer();
				AssertThat(inst.refCount(), Equals(1));

				auto pinst = instance<>::fromInternalPointer(p);
				AssertThat(inst.get(), Equals(pinst.get()));
				AssertThat(inst.refCount(), Equals(2));
				AssertThat(pinst.refCount(), Equals(2));
			});
		});

	});
});
