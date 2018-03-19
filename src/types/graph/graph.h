#pragma once
#include "../common.h"
#include "../exceptions.hpp"

namespace craft {
namespace types {
	class TypeGraph;

	class SubtypeAlgorithmState;
	class ValidationAlgorithmState;
	class ValidationWithSubtypingAlgorithmState;
}}

#include "RootType.hpp"

#include "TupleType.h"
#include "UnionType.h"
#include "VariantType.h"

#include "TypeVariableType.h"
#include "ExistsType.h"

#include "FunctionType.h"

#include "TagAbstractType.h"

#include "TypeGraph.h"
