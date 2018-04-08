#pragma once
#include "common.h"

CRAFT_MULTIMETHOD_DECLARE(test_multimethod_simple, craft::types::SimpleDispatcher);

CRAFT_MULTIMETHOD_DECLARE(test_multimethod_expression, craft::types::ExpressionDispatcher);

CRAFT_MULTIMETHOD_DECLARE(test_multimethod_instance, craft::types::WithInstancesDispatcher<craft::types::ExpressionDispatcher>);
