#include "../common.h"
#include "../core.h"

using namespace craft::types;

// Core Providers
CRAFT_DEFINE(PConstructor) { _.defaults(); };
CRAFT_DEFINE(PStringer) { _.defaults(); };
CRAFT_DEFINE(PRepr) { _.defaults(); };
CRAFT_DEFINE(PConversion) { _.defaults(); };
CRAFT_DEFINE(PParse) { _.defaults(); };
CRAFT_DEFINE(PClone) { _.defaults(); };

// Object Providers
CRAFT_DEFINE(PObjectContextual) { _.defaults(); };

// Core Aspects
CRAFT_DEFINE(SLifecycle) { _.defaults(); };
CRAFT_DEFINE(SContainer) { _.defaults(); };

// Object Aspects
CRAFT_DEFINE(SObjectComposite) { _.defaults(); };
CRAFT_DEFINE(SObjectManipulation) { _.defaults(); };
CRAFT_DEFINE(SObjectFungible) { _.defaults(); };
