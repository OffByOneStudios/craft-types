#pragma once
#include "common.h"
#include "core.h"

namespace craft {
namespace types
{
	enum class FeatureKind
	{
		Provider,
		Aspect
	};

	/******************************************************************************
	** IFeatureManager
	******************************************************************************/

	class IFeatureManager abstract
	{
	public:
		virtual ~IFeatureManager() = default;

		virtual FeatureKind featureKind() const = 0;
		virtual TypeId featureId() const = 0;
		virtual std::string featureName() const = 0;

		// T:
		// The following features depend on the kind of the feature manager and may not function
		// quite as expected in some cases.

		// Will attempt to remove type from manager
		virtual void purgeType(TypeId) noexcept = 0;

		// Provides a list of types this feature supports
		virtual std::vector<TypeId> supportedTypes() const = 0;
	};

	/******************************************************************************
	** Feature
	******************************************************************************/

	/* An *Interface* in the type system

	*/
	class Feature abstract
	{
	public:
		virtual ~Feature() = default;

		virtual IFeatureManager* craft_featureManager() const = 0;
		virtual TypeId craft_typeId() const = 0;
	};

	/******************************************************************************
	** type
	******************************************************************************/

	template <typename TFeature>
	struct type<TFeature,
		typename std::enable_if< std::is_base_of<Feature, TFeature>::value && !std::is_base_of<Object, TFeature>::value >::type
	>
		: public type<void>
	{
		static constexpr bool isFeature = true;

		inline static TypeId typeId() { return TFeature::craft_s_typeId(); }
		inline static std::string name() { return TFeature::craft_c_name; }
	};
}}
