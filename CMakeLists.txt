cmake_minimum_required(VERSION 3.7)


# This Variable is the Simplest way to ensure your variables will work
include(cmake/ResolveCraftRoot.cmake)
# This is mandatory
include(cmake/ResolveCraftCmake.cmake)

MESSAGE(STATUS "CRAFT_CMAKE_DIR " ${CRAFT_CMAKE_DIR})
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

# Use This Preprocess Def to Control exports
add_definitions(-DCRAFT_TYPE_DLL)

# File Operations
file(GLOB_RECURSE includes
  src/*.h
  src/*.hpp
)

file(GLOB_RECURSE sources
  src/*.cpp
)

file(GLOB_RECURSE tests
  test/*.cpp
)

file(GLOB_RECURSE docs
  src/*.md
)

MESSAGE(STATUS "CRAFT_VENDOR_INCLUDE_DIRECTORY" ${CRAFT_VENDOR_INCLUDE_DIRECTORY})
include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CRAFT_VENDOR_INCLUDE_DIRECTORY}
)

link_directories(
  ${CRAFT_VENDOR_LIB_DIRECTORY}
)

add_library(CraftType SHARED
  ${sources}
  ${includes}
  common.cpp
)

add_precompiled_header(CraftType src/common.h FORCEINCLUDE)

add_executable(CraftTypeTests
  ${tests}
)

target_link_libraries (CraftTypeTests CraftType)

# Platform Specific
if(CRAFT_TRIPLE_CROSS STREQUAL "osx_x64_clang")
  list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++ -Wc++11-extensions")
else()
  set(files ${includes} ${sources} ${docs})
  VSFILTERS(${files})
endif()
