#pragma once

//
// Platform Dynamic Library
//
#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define CULTLANG_SYNDICATE_EXPORTED_EXPORT __attribute__ ((dllexport))
#define CULTLANG_SYNDICATE_EXPORTED_IMPORT __attribute__ ((dllimport))
#else
#define CULTLANG_SYNDICATE_EXPORTED_EXPORT __declspec(dllexport)
#define CULTLANG_SYNDICATE_EXPORTED_IMPORT __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define CULTLANG_SYNDICATE_EXPORTED_EXPORT __attribute__ ((visibility ("default")))
#define CULTLANG_SYNDICATE_EXPORTED_IMPORT
#else
#define CULTLANG_SYNDICATE_EXPORTED_EXPORT
#define CULTLANG_SYNDICATE_EXPORTED_IMPORT
#endif
#endif

//
// Compiler fixups
//

#if !defined(_MSC_VER) || defined(__clang__)
#define abstract
#endif

//
// Engine Compile
//

#ifdef CULTLANG_SYNDICATE_DLL
#define CULTLANG_SYNDICATE_EXPORTED CULTLANG_SYNDICATE_EXPORTED_EXPORT
#else
#define CULTLANG_SYNDICATE_EXPORTED CULTLANG_SYNDICATE_EXPORTED_IMPORT
#endif

#ifdef __GNUC__
#define CRAFT_INIT_PRIORITY __attribute__((init_priority (1500)))
#else
#define CRAFT_INIT_PRIORITY
#endif
