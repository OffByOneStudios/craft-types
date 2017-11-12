
cmake_minimum_required(VERSION 3.7)
if(TARGET CraftEngineDepsTypes)
    return()
endif(TARGET CraftEngineDepsTypes)
Project(CraftEngineDepsTypes)

IF(NOT DEFINED CRAFT_ROOT_PROJECT)
	SET(CRAFT_ROOT_PROJECT CraftEngineDepsTypes)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ENDIF()

# This Variable is the Simplest way to ensure your variables will work
include(cmake/ResolveCraftRoot.cmake)
# This is mandatory
include(cmake/ResolveCraftCmake.cmake)

# Use This To Support Multiplatforms or to use the Vendor Repo.
include(${CRAFT_CMAKE_DIR}/ResolveCraftTripleCross.cmake)


include(${CRAFT_CMAKE_DIR}/ResolveVendor.cmake)
# Use This is you depend on Vendors
IF(NOT DEFINED CRAFT_ENGINE_VENDOR_DIR)
  MESSAGE(FATAL_ERROR "Could not locate required dependency CRAFT_ENGINE_VENDOR_DIR")
ENDIF()


# External Cmake Deps
include(${CRAFT_CMAKE_DIR}/PrecompiledHeader.cmake)
include(${CRAFT_CMAKE_DIR}/MakeVSFilters.cmake)
include(${CRAFT_CMAKE_DIR}/MakeVSDocs.cmake)

# Use This Preprocess Def to Control exports
add_definitions(-D${CRAFT_TRIPLE_CROSS})

# Dependent Projects
add_subdirectory(${CRAFT_ENGINE_ROOT_DIR}/deps/util CraftEngineDepsUtil)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CRAFT_VENDOR_INCLUDE_DIRECTORY}
  ${CRAFT_ENGINE_ROOT_DIR}/deps/util/src
  
)

# File Operations
set(LibSources
	src/types/all.h
  src/types/Aspect.h
  src/types/common.h
  src/types/Context.cpp
  src/types/Context.h
  src/types/core.cpp
  src/types/core.h
  src/types/default_features.cpp
  src/types/default_types_c.cpp
  src/types/default_types_c.h
  src/types/default_types_cpp.cpp
  src/types/default_types_cpp.h
  src/types/default_types_mpir.cpp
  src/types/default_types_mpir.h
  src/types/defines.h
  src/types/exceptions.hpp
  src/types/FactoryAspectManager.hpp
  src/types/Feature.h
  src/types/helpers.h
  src/types/Instance.hpp
  src/types/InstanceAspectManager.hpp
  src/types/instance_prelude.hpp
  src/types/Object.h
  src/types/ObjectDefineDetails.hpp
  src/types/PClone.h
  src/types/PConstructor.h
  src/types/PConversion.h
  src/types/PIdentifier.h
  src/types/PObjectContextual.h
  src/types/PParse.h
  src/types/prelude.h
  src/types/Provider.h
  src/types/PStringer.h
  src/types/README.md
  src/types/SContainer.h
  src/types/signal.hpp
  src/types/SingletonProviderManager.hpp
  src/types/SLifecycle.h
  src/types/SObjectComposite.h
  src/types/SObjectFungible.h
  src/types/SObjectManipulation.h
  src/types/to_string.cpp
  src/types/Type.h
  src/types/Types.h
  
)

set(TestSources
	test/adv_object.cpp
  test/adv_object.h
  test/basic_object.cpp
  test/basic_object.h
  test/common.h
  test/empty_aspects.cpp
  test/empty_aspects.h
  test/empty_providers.cpp
  test/empty_providers.h
  test/external_struct.cpp
  test/external_struct.h
  test/main.cpp
  test/test_Context.cpp
  test/test_defaults.cpp
  test/test_features.cpp
  test/test_instance.cpp
  test/test_lifecycle.cpp
  test/test_PConversion.cpp
  test/test_SObjectComposite.cpp
  test/test_SObjectFungible.cpp
  test/test_SObjectManipulation.cpp
  test/test_std_c_types.cpp
  test/test_std_string.cpp
  test/test_to_string.cpp
  
)

set(EntrySources
	entry/main.cpp
  
)


link_directories(
  ${CRAFT_VENDOR_LIB_DIRECTORY}
)

add_library(CraftEngineDepsTypes SHARED
  ${LibSources}
  common.cpp
)

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;Trace")
SET(CMAKE_CXX_FLAGS_TRACE ${CMAKE_CXX_FLAGS_DEBUG})
SET(CMAKE_LINKER_FLAGS_TRACE ${CMAKE_LINKER_FLAGS_DEBUG})
SET(CMAKE_SHARED_LINKER_FLAGS_TRACE ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
SET(CMAKE_EXE_LINKER_FLAGS_TRACE ${CMAKE_EXE_LINKER_FLAGS_DEBUG})

set_property(TARGET CraftEngineDepsTypes
  APPEND PROPERTY COMPILE_DEFINITIONS
  $<$<CONFIG:Trace>:BUILD_TRACE>
  $<$<CONFIG:Debug>:BUILD_DEBUG>
  $<$<CONFIG:Release>:BUILD_RELEASE>
)


target_compile_definitions(CraftEngineDepsTypes PRIVATE CRAFT_TYPES_DLL)

add_precompiled_header(CraftEngineDepsTypes src/types/common.h FORCEINCLUDE)

target_link_libraries (CraftEngineDepsTypes CraftEngineDepsUtil  )

# Tests
IF(${CRAFT_ROOT_PROJECT} STREQUAL CraftEngineDepsTypes)
	add_executable(CraftEngineDepsTypesTest ${TestSources})
	target_link_libraries(CraftEngineDepsTypesTest CraftEngineDepsTypes)
ENDIF()
# Entry
IF(${CRAFT_ROOT_PROJECT} STREQUAL CraftEngineDepsTypes)
	add_executable(CraftEngineDepsTypesEntry ${EntrySources})
	target_link_libraries(CraftEngineDepsTypesEntry CraftEngineDepsTypes)
ENDIF()

# IDE
set(files ${LibSources} ${TestSources} ${EntrySources})
VSFILTERS(${files})

#Vendor Dependencies
include(cmake/vendor/${CRAFT_TRIPLE_CROSS}.cmake)

# Platform Specific
include(cmake/platform/${CRAFT_TRIPLE_CROSS}.cmake)
include(cmake/flags/${CRAFT_TRIPLE_CROSS}.cmake)
include(cmake/defines/${CRAFT_TRIPLE_CROSS}.cmake)
