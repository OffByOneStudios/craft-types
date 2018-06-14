
#include "basic_object.h"

CRAFT_DEFINE(TestBasicObject)
{
	_.defaults();
}

TestBasicObject::TestBasicObject()
{

}

TestBasicObject::TestBasicObject(TestBasicObject const&)
{

}

bool TestBasicObject::sameAs_string(TestBasicObject const* that) const
{
	return this->a_string == that->a_string;
}