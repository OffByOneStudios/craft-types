#include "../../common.h"
#include "../../core.h"
#include "expression.h"
#include "../../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

/******************************************************************************
** Expression Specials
******************************************************************************/

ExpressionAny ExpressionAny::Value;
CRAFT_TYPE_DEFINE(ExpressionAny) { _.defaults(); }

TypeId ExpressionAny::kind() const { return cpptype<ExpressionAny>::typeDesc(); }
void* ExpressionAny::ptr() const { return (void*)this; }

std::string ExpressionAny::displayString() const { return "Any"; }
std::vector<IExpression*> const* ExpressionAny::children() const { return nullptr; }

void ExpressionAny::destroy() { }
IExpression* ExpressionAny::clone() const { return &Value; }


ExpressionVoid ExpressionVoid::Value;
CRAFT_TYPE_DEFINE(ExpressionVoid) { _.defaults(); }

TypeId ExpressionVoid::kind() const { return cpptype<ExpressionVoid>::typeDesc(); }
void* ExpressionVoid::ptr() const { return (void*)this; }

std::string ExpressionVoid::displayString() const { return "Void"; }
std::vector<IExpression*> const* ExpressionVoid::children() const { return nullptr; }

void ExpressionVoid::destroy() { }
IExpression* ExpressionVoid::clone() const { return &Value; }


ExpressionBottom ExpressionBottom::Value;
CRAFT_TYPE_DEFINE(ExpressionBottom) { _.defaults(); }

TypeId ExpressionBottom::kind() const { return cpptype<ExpressionBottom>::typeDesc(); }
void* ExpressionBottom::ptr() const { return (void*)this; }

std::string ExpressionBottom::displayString() const { return "Bottom"; }
std::vector<IExpression*> const* ExpressionBottom::children() const { return nullptr; }

void ExpressionBottom::destroy() { }
IExpression* ExpressionBottom::clone() const { return &Value; }



/******************************************************************************
** ExpressionConcrete
******************************************************************************/

CRAFT_TYPE_DEFINE(ExpressionConcrete)
{
	_.defaults();
}

ExpressionConcrete::ExpressionConcrete(TypeId tid)
	: IExpression()
	, node(tid.node)
{

}

ExpressionConcrete::~ExpressionConcrete()
{

}

TypeId ExpressionConcrete::kind() const
{
	return cpptype<ExpressionConcrete>::typeDesc();
}
void* ExpressionConcrete::ptr() const
{
	return (void*)this;
}

std::string ExpressionConcrete::displayString() const
{
	return TypeId(node).toString();
}
std::vector<IExpression*> const* ExpressionConcrete::children() const
{
	return nullptr;
}

void ExpressionConcrete::destroy()
{
	delete this;
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
	_.defaults();
}

ExpressionArrow::ExpressionArrow(IExpression* input, IExpression* output)
	: IExpression()
	, input(input)
	, output(output)
{

}

ExpressionArrow::~ExpressionArrow()
{
	input->destroy();
	output->destroy();
}

TypeId ExpressionArrow::kind() const
{
	return cpptype<ExpressionArrow>::typeDesc();
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

void ExpressionArrow::destroy() 
{
	delete this;
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
	_.defaults();
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
		e->destroy();

	if (varType != nullptr)
		varType->destroy();
}

TypeId ExpressionTuple::kind() const
{
	return cpptype<ExpressionTuple>::typeDesc();
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
	return &entries;
}

void ExpressionTuple::destroy()
{
	delete this;
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
	_.defaults();
}

ExpressionStore::ExpressionStore()
{
	this->_building = true;
}
ExpressionStore::ExpressionStore(IExpression* ptr)
{
	this->_root = std::shared_ptr<IExpression>(ptr, [=](IExpression* ex) { ex->destroy(); });
	this->_building = false;
}
ExpressionStore::ExpressionStore(ExpressionStore const& other)
{
	this->_root = other._root;
	this->_building = true;
}
ExpressionStore::~ExpressionStore()
{
}

void ExpressionStore::finish()
{
	this->_building = false;
}

IExpression const* ExpressionStore::root() const
{
	return _root.get();
}

