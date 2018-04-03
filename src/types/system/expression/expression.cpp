#include "../../common.h"
#include "../../core.h"
#include "expression.h"

using namespace craft;
using namespace craft::types;

Expression::~Expression()
{

}

/******************************************************************************
** ExpressionConcrete
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionConcrete)
{

}

ExpressionConcrete::ExpressionConcrete(Graph::Node const& node)
	: Expression()
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
std::string const& ExpressionConcrete::displayString() const
{
	identifiers();
	return ""; // TODO
}
std::vector<Expression*> const* ExpressionConcrete::children() const
{
	return nullptr;
}

Expression* ExpressionConcrete::clone() const
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

Expression const* ExpressionStore::root()
{
	return _root;
}