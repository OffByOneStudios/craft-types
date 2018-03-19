#pragma once
#include "common.h"

// Forward Declarations
namespace craft {
namespace types
{
	class Identifiers;
	inline Identifiers& identifiers();

	class TypeGraph;
	inline TypeGraph& graph();

	class CppSystem;
	inline CppSystem& system();
}}

//
// Components Subsystems
//

#include "identifiers.h"
#include "graph/graph.h"

//
// Forward declares
//

#include "prelude.h"
#include "exceptions.hpp"

//
// Cpp System
//

#include "cpp/instance_prelude.hpp"

#include "cpp/cpp_interface.h"

#include "cpp/SingletonProviderManager.hpp"

#include "cpp/FactoryAspectManager.hpp"
#include "cpp/InstanceAspectManager.hpp"

#include "cpp/Instance.hpp"

/*
#include "Feature.h"
#include "Object.h"

#include "Provider.h"
#include "Aspect.h"


#include "Types.h"
*/

//
// Templated defines
//

/*


#include "Type.h"
*/

//
// Feature helpers
//

#include "boot/Context.h"

//
// Built in features
//

#include "boot/PIdentifier.h"
#include "boot/PConstructor.h"
#include "boot/PStringer.h"
#include "boot/PParse.h"
#include "boot/PRepr.h"
#include "boot/PConversion.h"
#include "boot/PClone.h"
#include "boot/SLifecycle.h"
#include "boot/SContainer.h"

#include "boot/PObjectContextual.h"
#include "boot/SObjectComposite.h"
#include "boot/SObjectManipulation.h"
#include "boot/SObjectFungible.h"

//
// Helpers
//

#include "helpers.h"
#include "cpp/TypeDefineHelper.hpp"

//
// Built in types
//

#include "boot/default_types_c.h"
#include "boot/default_types_cpp.h"
#include "boot/default_types_mpir.h"

// TODO:
// * Rework objects to manage their own instance meta stuff (rather than from_raw) via SLifecycle
// * Hookup ->byConfiguring
// * Finish ObjectComposite and ObjectManipulation capabilities.
// * Rename ObjectDefineDetails to take into account types
// * Add default c_types
// * Add default cpp_types
