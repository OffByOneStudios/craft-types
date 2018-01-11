#include "../common.h"
#include "graph.h"
#include "BaseType.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** BaseType
******************************************************************************/

BaseType::BaseType()
{
	_identifier = None;
}
BaseType::~BaseType() { }

std::string BaseType::identifiableName() const { throw type_graph_not_implemented_error("BaseType::identifiableName"); }
TypeId BaseType::identifiableId() const { return _identifier; }

bool BaseType::isParametricScope() const { return false; }
bool BaseType::isParametricConsumer() const { return false; }
std::vector<SType*> const& BaseType::parametricScopes() const { throw type_graph_not_implemented_error("BaseType::parametricScopes"); }
std::vector<SType*> const& BaseType::parametricConsumption() const { throw type_graph_not_implemented_error("BaseType::parametricConsumption"); }

bool BaseType::isSubtypeHierarchyParticipant() const { return false; }
bool BaseType::canHaveChildren() const { return false; }
bool BaseType::canHaveAdoptiveParents() const { return false; }

std::vector<SType*> const& BaseType::parents() const { throw type_graph_not_implemented_error("BaseType::parents"); }
std::set<SType*> const& BaseType::adoptiveParents() const { throw type_graph_not_implemented_error("BaseType::adoptiveParents"); }
std::set<SType*> const& BaseType::children() const { throw type_graph_not_implemented_error("BaseType::children"); }

void BaseType::makeChild(TypeGraphSystem*, SType*) { throw type_graph_not_implemented_error("BaseType::makeChild"); }
void BaseType::adoptiveParent(TypeGraphSystem*, SType*) { throw type_graph_not_implemented_error("BaseType::adoptiveParent"); }

void BaseType::subtypingDo(SubtypeAlgorithmState*) const { }
void BaseType::validationDo(ValidationAlgorithmState*) const { }
void BaseType::validationDo(ValidationWithSubtypingAlgorithmState*) const { }

void BaseType::_registerTypeIdentifier(TypeId const& id)
{
	_identifier = id;
}

/******************************************************************************
** BaseHierarchyType
******************************************************************************/

BaseHierarchyType::~BaseHierarchyType()
{

}

bool BaseHierarchyType::isSubtypeHierarchyParticipant() const { return true; }
bool BaseHierarchyType::canHaveChildren() const { return true; }
bool BaseHierarchyType::canHaveAdoptiveParents() const { return true; }

std::vector<SType*> const& BaseHierarchyType::parents() const
{
	return _hierarchy_parents;
}
std::set<SType*> const& BaseHierarchyType::adoptiveParents() const
{
	return _hierarchy_adoptiveParents;
}
std::set<SType*> const& BaseHierarchyType::children() const
{
	return _hierarchy_children;
}

void BaseHierarchyType::makeChild(TypeGraphSystem* sys, SType* new_child)
{
	std::lock_guard<std::recursive_mutex> lock(sys->operation);


}
void BaseHierarchyType::adoptiveParent(TypeGraphSystem* sys, SType* new_parent)
{
	std::lock_guard<std::recursive_mutex> lock(sys->operation);


}

void BaseHierarchyType::subtypingDo(SubtypeAlgorithmState*) const
{
}
void BaseHierarchyType::validationDo(ValidationAlgorithmState*) const
{
}
void BaseHierarchyType::validationDo(ValidationWithSubtypingAlgorithmState*) const
{
}
