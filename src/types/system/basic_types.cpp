#include "../common.h"
#include "../core.h"
#include "basic_types.h"
#include "../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

CRAFT_TYPE_DEFINE(Type_Property_SystemGraphKind) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Property_NamespaceIdentifier) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Property_LocalIdentifier) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Node_AbstractType) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Edge_IsA) { _.defaults(); }
