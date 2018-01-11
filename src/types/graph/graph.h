#pragma once
#include "../common.h"
#include "../exceptions.hpp"
#include "graph.h"

namespace craft {
namespace types {
	class TypeGraphSystem;

	class SubtypeAlgorithmState;
	class ValidationAlgorithmState;
	class ValidationWithSubtypingAlgorithmState;
}}

#include "TypeId.hpp"

#include "SType.hpp"

#include "TupleType.h"
#include "UnionType.h"
#include "VariantType.h"

#include "TypeVariableType.h"
#include "ExistsType.h"

#include "FunctionType.h"

#include "BaseType.h"

#include "TagAbstractType.h"

#include "CppNativeAbstractType.h"
#include "CppNativeActualType.h"
#include "CppObjectActualType.h"

#include "TypeGraphSystem.h"