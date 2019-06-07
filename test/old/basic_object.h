#pragma once
#include "types/common.h"

class TestBasicObject
	: public virtual craft::types::Object
{
	CRAFT_OBJECT_DECLARE(TestBasicObject);
public: // normally private

	std::string a_string;
	uint64_t a_int;
	double a_double;

public:
	TestBasicObject();
	TestBasicObject(TestBasicObject const&);

	bool sameAs_string(TestBasicObject const*) const;
};
