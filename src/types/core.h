#pragma once
#include "common.h"

//
// Forward declares
//

#include "prelude.h"
#include "exceptions.hpp"

#include "Feature.h"
#include "Object.h"

#include "Provider.h"
#include "Aspect.h"

#include "instance_prelude.hpp"

#include "Types.h"

//
// Templated defines
//

#include "instance.hpp"

#include "SingletonProviderManager.hpp"

#include "FactoryAspectManager.hpp"
#include "InstanceAspectManager.hpp"

#include "Type.h"

//
// Feature helpers
//

#include "Context.h"

//
// Built in features
//

#include "PIdentifier.h"
#include "PConstructor.h"
#include "PStringer.h"
#include "PParse.h"
#include "PConversion.h"
#include "PClone.h"
#include "SLifecycle.h"
#include "SContainer.h"

#include "PObjectContextual.h"
#include "SObjectComposite.h"
#include "SObjectManipulation.h"
#include "SObjectFungible.h"

//
// Helpers
//

#include "helpers.h"
#include "ObjectDefineDetails.hpp"

//
// Built in types
//

#include "default_types_c.h"
#include "default_types_cpp.h"

// TODO:
// * Rework objects to manage their own instance meta stuff (rather than from_raw) via SLifecycle
// * Hookup ->byConfiguring
// * Finish ObjectComposite and ObjectManipulation capabilities.
// * Rename ObjectDefineDetails to take into account types
// * Add default c_types
// * Add default cpp_types
