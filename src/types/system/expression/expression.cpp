#include "../../common.h"
#include "../../core.h"
#include "expression.h"
#include "../../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

IExpression::~IExpression()
{

}

/******************************************************************************
** ExpressionConcrete
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionConcrete)
{

}

ExpressionConcrete::ExpressionConcrete(Graph::Node const& node)
	: IExpression()
	, node(node)
{

}

ExpressionConcrete::~ExpressionConcrete()
{

}

Graph::Node ExpressionConcrete::kind() const
{
	return graph().get<ExpressionConcrete>();
}
void* ExpressionConcrete::ptr() const
{
	return (void*)this;
}

std::string const& ExpressionConcrete::displayString() const
{
	identifiers();
	return ""; // TODO
}
std::vector<IExpression*> const* ExpressionConcrete::children() const
{
	return nullptr;
}

IExpression* ExpressionConcrete::clone() const
{
	return new ExpressionConcrete(node);
}

/******************************************************************************
** ExpressionStore
******************************************************************************/

ExpressionStore::ExpressionStore()
{
	this->_building = true;
}
ExpressionStore::ExpressionStore(ExpressionStore const& other)
{
	this->_root = other._root->clone();
	this->_building = true;
}
ExpressionStore::~ExpressionStore()
{
	if (this->_root != nullptr)
		delete this->_root;
}

void ExpressionStore::finish()
{
	this->_building = false;
}

IExpression const* ExpressionStore::root()
{
	return _root;
}