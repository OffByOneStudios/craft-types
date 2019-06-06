
/*
	In this file we demonstrate adding type information to existing C++ types.
*/

/******************************************************************************
** External Header File
******************************************************************************/

/*
	First we define a couple of plain C++ types:
*/

struct Basic
{
public:
	int memberValue;

public:
	float basicFunction(float);
};

struct BasicChild
	: public Basic
{
public:
	int memberValueAgain;

public:
	float basicFunction(float); // a new implementation
	int basicFunction(int); // an overload implementation
};

struct Advanced
	: public Basic
{
public:
	virtual char const* virtualFunction();
};

struct AdvancedOverride
	: public Advanced
{
public:
	virtual char const* virtualFunction() override;
};

struct AdvancedMulti
	: public AdvancedOverride, public BasicChild
{
public:
	virtual char const* virtualFunction() override;
};

/******************************************************************************
** External Code File
******************************************************************************/

/*
	And some code files.
*/

float Basic::basicFunction(float a) { return a + a; }

float BasicChild::basicFunction(float a) { return a * a; }
int BasicChild::basicFunction(int a) { return a * a; }

char const* Advanced::virtualFunction() { return "Advanced"; }

char const* AdvancedOverride::virtualFunction() { return "AdvacedOverride"; }

char const* AdvancedMulti::virtualFunction() { return "AdvacedMulti"; }

/******************************************************************************
** Our Header File
******************************************************************************/

/*
	Next we write our header file. First we include the type system for the first time.
*/

#include "types/core.h"
	
/*
	And now we will attach the type system data to the "external" types we defined earlier.

	Putting the following in a header file is optional to get the runtime features of the type
	graph, however to access the compile time helpers of the type system these declarations must
	be visible.

	However, they must be placed in the root namespace. The first argument is an optional DLL macro
	helper.
*/

CRAFT_TYPE_DECLARE(, Basic);
CRAFT_TYPE_DECLARE(, BasicChild);
CRAFT_TYPE_DECLARE(, Advanced);
CRAFT_TYPE_DECLARE(, AdvancedOverride);
CRAFT_TYPE_DECLARE(, AdvancedMulti);

/******************************************************************************
** Our Code File
******************************************************************************/

/*
	Finally we write the type graph definitions using our fluent interface [0] DSL.

	[0] https://martinfowler.com/bliki/FluentInterface.html
*/

/*
	To access the fluent interface we use the `_` symbol. A decent amount of automatic
	introspection is availble through the `defaults()` function. Primarily it adds default names
	and pulls in the more important C++ overloads (constructors, destructors, assignment) and other
	information (sizeof). Defaults are usually called last (incase we override a feature it default
	sets earlier on), and it should always be called.
	
	Default names are calculated using the symbol from the define. The type system is capable of
	intepreting C++ namespaces, so we will prefer fully qualified names (e.g. `foo::bar::Thing`).

	We can then add member values and member functions.
*/

CRAFT_TYPE_DEFINE(Basic)
{
	_.member("memberValue", &Basic::memberValue);
	_.member("basicFunction", &Basic::basicFunction);

	_.defaults();
};

/*
	We can define a parent class, which will allow external resolutions to use it's members.

	Note here how we have to specialize the member template to be specific about function
	overloads. Signature based function overloads can be used because these are member functions
	instead a classic generic style is used (`_.member<retType, arg0Type, arg1Type>()`).
*/

CRAFT_TYPE_DEFINE(BasicChild)
{
	_.parent<Basic>();

	_.member("memberValueAgain", &BasicChild::memberValueAgain);
	_.member<int>("basicFunction", &BasicChild::basicFunction);
	_.member<float>("basicFunction", &BasicChild::basicFunction);

	_.defaults();
};

/*
	Virtual member functions can also be defined.

	Here we also show overriding the names by passing a C++ style type name.
*/

CRAFT_TYPE_DEFINE(Advanced)
{
	_.parent<Basic>();

	_.member("virtualFunction", &Advanced::virtualFunction);

	_.identify_asEffectiveCppName("example0::Advanced");
	_.defaults();
};

/*
	Virtual member functions can also be defined.

	Here we also show overriding the names explictly.
*/

CRAFT_TYPE_DEFINE(AdvancedMulti)
{
	_.parents<Advanced, BasicChild>();

	_.identify_verbose("example0/AdvancedMulti@Graph", "example0/AdvancedMulti", "example0::AdvancedMulti");
	_.defaults();
};
