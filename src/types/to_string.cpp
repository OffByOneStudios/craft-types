#include "helpers.h"

using namespace craft::types;

std::string TypeId::toString(bool verbose) const
{
	if (id == None.id)
	{
		if (verbose)
			return "<Type|None>";
		else
			return "None";
	}

	std::string from;
	std::string name;

	if (hasFeature<PIdentifier>())
	{
		from = PIdentifier::craft_c_featureName;
		name = getFeature<PIdentifier>()->identifier();
	}
	else
	{
		from = "#";
		name = std::to_string(id);
	}

	if (verbose)
		return fmt::format("<Type|{0}|{1}>", from, name);
	else
		return name;
}

std::string FeatureId::toString(bool verbose) const
{
	if (id == NoFeature.id)
	{
		if (verbose)
			return "<Feature|NoFeature>";
		else
			return "NoFeature";
	}

	if (verbose)
		return fmt::format("<Feature|name (#)|{0} ({1})>", getManager()->featureName(), id);
	else
		return fmt::format("{0} ({1})", getManager()->featureName(), id);
}
