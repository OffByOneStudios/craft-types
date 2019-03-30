#include "../common.h"
#include "../core.h"
#include "native_types.h"
#include "../boot/system_into_cpp.h"

using namespace craft;
using namespace craft::types;

CRAFT_TYPE_DEFINE(Type_Node_BitsType) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Node_StructuralType) { _.defaults(); }
CRAFT_TYPE_DEFINE(Type_Property_NativeSize) { _.defaults(); }
