#pragma once
#include "syn/common.h"
#include "empty_aspects.h"
#include "empty_providers.h"
#include "basic_object.h"

class TestAdvancedObject
	: public virtual syn::Object
{
	CRAFT_OBJECT_DECLARE(TestAdvancedObject);
public: // normally private

	craft::instance<> a_generic_instance;
	craft::instance<TestBasicObject> a_concrete_instance;
	craft::instance<SEmptyTestFactory> a_aspect_instance;
	craft::instance<PEmptyTestSingleton> a_provider_instance;

public:
	TestAdvancedObject();
	TestAdvancedObject(TestAdvancedObject const&);
};
