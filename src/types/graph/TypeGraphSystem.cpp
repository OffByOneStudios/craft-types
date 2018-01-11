#include "../common.h"
#include "graph.h"
#include "TypeGraphSystem.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** TypeGraphSystem
******************************************************************************/

TypeGraphSystem::TypeGraphSystem()
{
	// this->add(new AnyType());
}
TypeGraphSystem::TypeGraphSystem(TypeGraphSystem const& that)
{

}
TypeGraphSystem::~TypeGraphSystem()
{

}

TypeId TypeGraphSystem::anyType() const
{
	return TypeId(1);
}
size_t TypeGraphSystem::typeCount() const
{
	return _types.size();
}

TypeId TypeGraphSystem::add(SType* type)
{
	std::lock_guard<std::recursive_mutex> lock(operation);

	_types.push_back(type);

	return TypeId(_types.size() - 1);
}
SType const* TypeGraphSystem::get(TypeId const& id) const
{
	return _types[id.id - 1];
}