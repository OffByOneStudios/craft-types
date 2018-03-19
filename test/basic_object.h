#pragma once
#include "common.h"

class TestBasicObject
	: public virtual craft::types::Object
{
	CRAFT_OBJECT_DECLARE(TestBasicObject);
private:

public:
	TestBasicObject();
	TestBasicObject(TestBasicObject const&);
};
