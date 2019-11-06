#pragma once
#include "syn/common.h"

// exception types
#include "exceptions.hpp"

/******************************************************************************
** Runtime Library
******************************************************************************/

/* Instance Data Structures (section 1.1) */
#include "runtime/instance.h"

/******************************************************************************
** System
******************************************************************************/

// prelude ////////////////////////////////////////////////////////////////////

/* system prelude:
    * Forward declares
    * Exceptions
*/
#include "system/prelude.h" 

/* cpp-support prelude:
    * Forward declares
    * Instance prelude
    * Type Template Lookups declarations
*/
#include "cpp/prelude_system.h" 


// stores /////////////////////////////////////////////////////////////////////

/* Symbol Table (section 1.2) */
#include "system/Symbol.hpp"
#include "system/SymbolTable.h"

/* Graph design (section 2.1) */
#include "system/Graph.hpp"

#include "system/TypeStore.h"
#include "system/TypeId.h"

#include "system/dispatch.h"

#include "system/ModuleBase.h"

// graph //////////////////////////////////////////////////////////////////////

/* Core Graph (section 2.2) */
#include "core/system_graph.h"

/* Syndicate Graph (section 2.3) */
#include "core/module_graph.h"
#include "core/syndicate_graph.h"

#include "system/epilogue.h"


/******************************************************************************
** C++ Support
******************************************************************************/

// prelude ////////////////////////////////////////////////////////////////////

#include "cpp/prelude.h" 

// instance ///////////////////////////////////////////////////////////////////

/* C++ instance templates (section 3.2) */
#include "cpp/instance/prelude.hpp"
#include "cpp/instance/policies.hpp"
#include "cpp/instance/containers.hpp"

// system /////////////////////////////////////////////////////////////////////

/* Define Helpers (section 3.1) */
#include "cpp/cpp_interface.h"

// graph //////////////////////////////////////////////////////////////////////

/* C++ graph and features (section 2.4) */
#include "core/cpp_graph.h"
#include "core/conversions.h"
#include "core/containers.h"
#include "core/numerics.h"

// dispatch ///////////////////////////////////////////////////////////////////

#include "cpp/dispatch/GenericInvoke.hpp"
#include "cpp/dispatch/BasicDispatcher.hpp"
//#include "cpp/dispatch/SimpleDispatcher.hpp"

/*

//#include "cpp/graph.hpp"
#include "cpp/cpp_types.h"
#include "cpp/expressions.hpp"
#include "system/ExpressionDispatcher.hpp"
*/

/******************************************************************************
** Boot Runtime Library
******************************************************************************/

/* Define Helpers (section 4.3) */
#include "cpp/DefineHelper.hpp"

#include "boot/default_types_c.h"
#include "boot/default_types_cpp.h"

#include "boot/system_into_cpp.h"


/******************************************************************************
** Dll Stuff
******************************************************************************/

#include "dll_helpers.h"
