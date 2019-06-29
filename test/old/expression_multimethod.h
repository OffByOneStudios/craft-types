#pragma once
#include "syn/common.h"

CRAFT_MULTIMETHOD_DECLARE(test_multimethod_expression, syn::ExpressionDispatcher);

CRAFT_MULTIMETHOD_DECLARE(test_multimethod_instance, syn::WithInstancesDispatcher<syn::ExpressionDispatcher>);
