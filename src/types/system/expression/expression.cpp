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
** ExpressionSpecial
******************************************************************************/

ExpressionSpecial* ExpressionSpecial::Void;
ExpressionSpecial* ExpressionSpecial::Any;
ExpressionSpecial* ExpressionSpecial::Bottom;

CRAFT_TYPE_DEFINE(ExpressionSpecial)
{
	ExpressionSpecial::Void = nullptr;
}

ExpressionSpecial::ExpressionSpecial(Graph::Node const& node)
	: IExpression()
	, node(node)
{

}

ExpressionSpecial::~ExpressionSpecial()
{

}

Graph::Node ExpressionSpecial::kind() const
{
	return graph().get<ExpressionConcrete>();
}
void* ExpressionSpecial::ptr() const
{
	return (void*)this;
}

std::string ExpressionSpecial::displayString() const
{
	return "";
}
std::vector<IExpression*> const* ExpressionSpecial::children() const
{
	return nullptr;
}

IExpression* ExpressionSpecial::clone() const
{
	return new ExpressionSpecial(node);
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

std::string ExpressionConcrete::displayString() const
{
	return identifiers().get(node.ptr()).id.toString();
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
** ExpressionArrow
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionArrow)
{

}

ExpressionArrow::ExpressionArrow(IExpression* input, IExpression* output)
	: IExpression()
	, input(input)
	, output(output)
{

}

ExpressionArrow::~ExpressionArrow()
{
	delete input;
	delete output;
}

Graph::Node ExpressionArrow::kind() const
{
	return graph().get<ExpressionArrow>();
}
void* ExpressionArrow::ptr() const
{
	return (void*)this;
}

std::string ExpressionArrow::displayString() const
{
	return fmt::format("{0} -> {1}", input->displayString(), input->displayString());
}
std::vector<IExpression*> const* ExpressionArrow::children() const
{
	return nullptr;
}

IExpression* ExpressionArrow::clone() const
{
	return new ExpressionArrow(input, output);
}


/******************************************************************************
** ExpressionTuple
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionTuple)
{

}

ExpressionTuple::ExpressionTuple(std::vector<IExpression*> const& entries, IExpression* varType)
	: IExpression()
	, entries(entries)
	, varType(varType)
{

}

ExpressionTuple::~ExpressionTuple()
{
	for (auto e : entries)
		delete e;

	if (varType != nullptr)
		delete varType;
}

Graph::Node ExpressionTuple::kind() const
{
	return graph().get<ExpressionConcrete>();
}
void* ExpressionTuple::ptr() const
{
	return (void*)this;
}

std::string ExpressionTuple::displayString() const
{
	return "";
}
std::vector<IExpression*> const* ExpressionTuple::children() const
{
	return nullptr;
}

IExpression* ExpressionTuple::clone() const
{
	return new ExpressionTuple(entries);
}

/******************************************************************************
** ExpressionStore
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionStore)
{

}

ExpressionStore::ExpressionStore()
{
	this->_building = true;
}
ExpressionStore::ExpressionStore(IExpression* ptr)
{
	this->_root = ptr;
	this->_building = false;
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