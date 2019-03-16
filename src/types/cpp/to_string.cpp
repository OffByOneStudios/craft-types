#include "../common.h"
#include "../core.h"

using namespace craft::types;

std::string instance<void>::toString(instance<void> const& inst, bool verbose)
{
	if (inst.typeId() != None)
		return inst.toString(inst, inst.typeId(), verbose);

	if (inst.isNull())
	{
		if (verbose)
			return "<instance|null>";
		else
			return "null";
	}

	if (verbose)
		return fmt::format("<instance|pointer|{0}>", stdext::to_hex_string(inst._meta->actual));
	else
		return fmt::format("0x{0}", stdext::to_hex_string(inst._meta->actual));
}

std::string instance<void>::toString(instance<void> const& inst, TypeId tid, bool verbose)
{
	if (inst.isNull())
	{
		std::string base = tid.toString();
		if (verbose)
			return fmt::format("<instance|type: {0}|null>"), base;
		else
			return fmt::format("{0} null", base);
	}

	std::string from, name;
	if (inst.hasFeature<PStringer>())
	{
		from = std::string(PStringer::craft_s_typeName());
		if (verbose)
			name = fmt::format("{0} {1}", tid.toString(), inst.getFeature<PStringer>()->toString(inst));
		else
			name = inst.getFeature<PStringer>()->toString(inst);
	}
	else
	{
		from = "typeId ptr";
		name = fmt::format("{0} 0x{1}", tid.toString(), stdext::to_hex_string(inst._meta->actual));
	}


	if (verbose)
		return fmt::format("<instance|{0}|{1}>", from, name);
	else
		return fmt::format("{0}", name);
}

/*
std::string instance<void>::toString(instance<void> const& inst, FeatureId fid, bool verbose)
{
	if (inst.isNull())
	{
		std::string base = fid.toString(false);
		if (verbose)
			return fmt::format("<instance|feature: {0}|null>"), base;
		else
			return fmt::format("null {0}", base);
	}

	if (inst.typeId() != None)
		return inst.toString(inst, inst.typeId(), verbose);

	return "TODO NOT IMPLEMENTED";
}
*/

std::string TypeId::toString() const
{
	if (_node == nullptr)
	{
		return "None";
	}

	std::string from, name;

	auto cpp_name = thread_store().onlyPropOfTypeOnNode<Type_Property_CppIdentifier>(_node);
	if (cpp_name)
	{
		return cpp_name->cpp_identifier;
	}

	auto ns_name = thread_store().onlyPropOfTypeOnNode<Type_Property_NamespaceIdentifier>(_node);
	if (ns_name)
	{
		return ns_name->namespace_identifier;
	}

	auto l_name = thread_store().onlyPropOfTypeOnNode<Type_Property_LocalIdentifier>(_node);
	if (l_name)
	{
		return l_name->local_identifier;
	}

	return fmt::format("Unknown-{0}", (void*)_node);
}