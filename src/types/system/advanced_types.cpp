#include "../common.h"
#include "../core.h"
#include "advanced_types.h"
#include "../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

CRAFT_TYPE_DEFINE(Type_Node_Multimethod) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Edge_MethodImplementation) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Node_GenericType) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Edge_GenericInstantiation) { _.defaults(); }
