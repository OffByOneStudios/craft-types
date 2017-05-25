#pragma once

//
// Platform Dynamic Library
//
#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define CRAFT_CMAKE_BASE_EXPORTED_EXPORT __attribute__ ((dllexport))
#define CRAFT_CMAKE_BASE_EXPORTED_IMPORT __attribute__ ((dllimport))
#else
#define CRAFT_CMAKE_BASE_EXPORTED_EXPORT __declspec(dllexport)
#define CRAFT_CMAKE_BASE_EXPORTED_IMPORT __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define CRAFT_CMAKE_BASE_EXPORTED_EXPORT __attribute__ ((visibility ("default")))
#define CRAFT_CMAKE_BASE_EXPORTED_IMPORT
#else
#define CRAFT_CMAKE_BASE_EXPORTED_EXPORT
#define CRAFT_CMAKE_BASE_EXPORTED_IMPORT
#endif
#endif


//
// Engine Compile
//

#ifdef CRAFT_CMAKE_BASE_DLL
#define CRAFT_CMAKE_BASE_EXPORTED CRAFT_CMAKE_BASE_EXPORTED_EXPORT
#else
#define CRAFT_CMAKE_BASE_EXPORTED CRAFT_CMAKE_BASE_EXPORTED_IMPORT
#endif
