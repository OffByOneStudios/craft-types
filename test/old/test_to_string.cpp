#include "syn/common.h"

#include "empty_aspects.h"
#include "empty_providers.h"

using namespace bandit;
using namespace snowhouse;

using namespace craft;

template<typename T>
struct feature_test_to_string
{
	void operator()()
	{
		it ("starts with `T::craft_c_featureName`", []() {
			AssertThat(T::craft_s_featureDesc().toString(false), StartsWith(T::craft_s_featureName()));
		});
	};
};

go_bandit([]()
{
	/*
	Testing using empty declarations of each feature manager
	*/
	describe_for_empty_providers<feature_test_to_string>("`FeatureId::toString`");
	describe_for_empty_aspects<feature_test_to_string>("`FeatureId::toString`");

	describe("TypeId::toString", []() {

	});
});
