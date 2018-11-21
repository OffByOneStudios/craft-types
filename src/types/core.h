#pragma once
#include "common.h"

//
// Forward declares
//

#include "prelude.h"
#include "exceptions.hpp"

//
// Core type system
//

#include "system/store.h" /* TypeStore, TypeId, TypeGraph (and dependents) */
#include "system/basic_types.h"
#include "system/expression/expression.h"
#include "system/expression/queries.h"
#include "system/native_types.h"
#include "system/advanced_types.h"
#include "system/epilogue.h"

//
// Cpp System
//

#include "cpp/instance_prelude.hpp"

#include "cpp/cpp_interface.h"

#include "cpp/SingletonProviderManager.hpp"

#include "cpp/FactoryAspectManager.hpp"
#include "cpp/InstanceAspectManager.hpp"

#include "cpp/Instance.hpp"
#include "system/GenericInvoke.hpp"

#include "system/SimpleDispatcher.hpp"

#include "boot/system_into_cpp.h"

//#include "cpp/graph.hpp"
#include "cpp/cpp_types.h"
#include "cpp/expressions.hpp"
#include "system/ExpressionDispatcher.hpp"

//
// Feature helpers
//

#include "boot/Context.h"

//
// Built in features
//

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
#include "cpp/DefineHelper.hpp"

//
// Built in types
//

#include "boot/default_types_c.h"
#include "boot/default_types_cpp.h"
// TODO:
// * Rework objects to manage their own instance meta stuff (rather than from_raw) via SLifecycle
// * Hookup ->byConfiguring
// * Finish ObjectComposite and ObjectManipulation capabilities.
// * Rename ObjectDefineDetails to take into account types
// * Add default c_types
// * Add default cpp_types
