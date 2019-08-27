# Bazel Build File For Type system

# https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=vs-2019
#   In case we have to link dlls ourselves apparently
# runtime
#"/DEFAULTLIB:ucrt.lib",
#"/DEFAULTLIB:vcruntime.lib",
#"/DEFAULTLIB:libvcruntime.lib",
#"/DEFAULTLIB:msvcrt.lib",
#"/DEFAULTLIB:msvcprt.lib",
# runtime (debug)
#"/DEFAULTLIB:ucrtd.lib",
#"/DEFAULTLIB:vcruntimed.lib",
#"/DEFAULTLIB:msvcrtd.lib",
#"/DEFAULTLIB:msvcprtd.lib",
#"/DEFAULTLIB:vcruntimed.lib",
# system
#"/DEFAULTLIB:kernel32.lib",
#"/DEFAULTLIB:user32.lib",
# other attempts
#"/DEFAULTLIB:oldnames.lib",
#"/clr"
# Was resolved by correclty compiling the /ENTRY: function


cc_library(
    name = "headers",
    visibility = ["//visibility:public"],

    hdrs = glob([
        "src/**/*.h*",
        "src/types/dll_entry.inc"
    ]),
    includes = [
        "src"
    ],

    deps = [
        "@graph//:graph",
        "@spdlog//:headers",
        "@stdext//:headers",
        "@boost//:callable_traits",
    ],

    linkopts = select({
        "@bazel_tools//src/conditions:windows": [
        ],
        "//conditions:default": [
            "-ldl",
            "-lpthread",
            "-lstdc++fs",
        ],
    }),
)

cc_library(
    name = "code",
    
    deps = [
        ":headers",
        "@stdext//:stdext",
    ],

    # temporary for development
    #srcs=glob([
    #    "src/**/*.c*"
    #]),
    srcs = [
        "src/syn/system/SymbolTable.cpp",
        "src/syn/system/TypeStore.cpp",
        "src/syn/system/TypeId.cpp",

        "src/syn/core/system_graph.cpp",
        "src/syn/core/cpp_graph.cpp",

        "src/syn/cpp/cpp_interface.cpp",

        "src/syn/boot/default_types_cpp.cpp",

        "src/syn/syn.cpp",
    ],

    # pending bazel 0.29 feature
    #local_defines = [
    #    "CULTLANG_TYPES_DLL",
    #    'CULT_CURRENT_PACKAGE=\"org_cultlang_' + 'syndicate' + '\"'
    #],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/MD", "/std:c++17", "/DCULTLANG_TYPES_DLL"],
        "//conditions:default": ["-std=c++17", "-DCULTLANG_TYPES_DLL"],
    }),
    
    alwayslink = True,
)

cc_binary(
    name = "Syndicate.dll",
    deps = [":code"],
    linkopts = [
        "/ENTRY:syn_DLLMAIN",
    ],
    linkshared = True,
)
# from: https://github.com/bazelbuild/bazel/blob/master/examples/windows/dll/windows_dll_library.bzl
filegroup(
    name = "Syndicate_import_lib",
    srcs = [":Syndicate.dll"],
    output_group = "interface_library",
)
cc_import(
    name = "Syndicate_dll_import",
    interface_library = ":Syndicate_import_lib",
    shared_library = ":Syndicate.dll",
)

cc_binary(
    name = "libSyndicate.dylib",
    deps = [":code"],
    linkshared = True,
)

cc_binary(
    name = "libSyndicate.so",
    deps = [":code"],
    linkshared = True,
)

cc_library(
    name = "syndicatefoo",
    visibility = ["//visibility:public"],

    deps = [
        ":headers"
    ] + select({
        "@bazel_tools//src/conditions:windows": ["Syndicate_dll_import"],
        "//conditions:default": [],
    }),

    srcs = select({
        "@bazel_tools//src/conditions:windows": [],
        "@bazel_tools//src/conditions:darwin": ["libSyndicate.dylib"],
        "//conditions:default": ["libSyndicate.so"],
    }),

    #alwayslink = True,
)

cc_binary(
    name = "syndicate_explorer",
    deps = [":syndicatefoo"],
    srcs = glob([
        "entry/*.c*",
        "entry/*.h*",
    ]),

    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17"],
        "//conditions:default": ["-std=c++17"],
    }),
)


cc_test(
    name = "unit",
    includes = ["test"],
    srcs = glob([
        "test/unit/**/*.cpp",
        "test/unit/**/*.h*"
    ]),
    deps = [
        ":syndicate",
        "@catch//:single_include",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17"],
        "//conditions:default": ["-std=c++17"],
    }),
)

cc_test(
    name = "literate",
    includes = ["test"],
    srcs = glob(["test/literate/*.cpp"]),
    deps = [
        ":syndicate",
       "@catch//:single_include",
    ],
)
