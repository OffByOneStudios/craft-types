# Bazel Build File For Type system


cc_library(
    name = "syndicate_hdrs",

    hdrs = glob([
        "src/**/*.h*",
        "src/types/dll_entry.inc"
    ]),
    includes = [
        "src"
    ],

    deps = [
        "@graph//:graph",
        "@stdext//:stdext",
        "@spdlog//:headers",
        "@boost//:callable_traits",
    ],

    linkopts = select({
        "@bazel_tools//src/conditions:windows": [],
        "//conditions:default": [
            "-ldl",
            "-lpthread",
            "-lstdc++fs",
        ],
    }),
)

cc_library(
    name = "syndicate_core",
    
    deps = [
        ":syndicate_hdrs"
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
    ],

    # pending bazel 0.29 feature
    #local_defines = [
    #    "CULTLANG_TYPES_DLL",
    #    'CULT_CURRENT_PACKAGE=\"org_cultlang_' + 'syndicate' + '\"'
    #],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17", "/DCULTLANG_TYPES_DLL"],
        "//conditions:default": ["-std=c++17", "-DCULTLANG_TYPES_DLL"],
    }),
    
    alwayslink = True,
)

cc_binary(
    name = "Syndicate.dll",
    deps = [":syndicate_core"],
    linkopts = [
        "/ENTRY:syn_DLLMAIN",
    ],
    linkshared = True,
)

cc_binary(
    name = "libSyndicate.dylib",
    deps = [":syndicate_core"],
    linkshared = True,
)

cc_binary(
    name = "libSyndicate.so",
    deps = [":syndicate_core"],
    linkshared = True,
)

cc_library(
    name = "syndicate",
    visibility = ["//visibility:public"],

    deps = [
        ":syndicate_hdrs"
    ],

    srcs = select({
        "@bazel_tools//src/conditions:windows": ["Syndicate.dll"],
        "@bazel_tools//src/conditions:darwin": ["libSyndicate.dylib"],
        "//conditions:default": ["libSyndicate.so"],
    }),

    alwayslink = True,
)

cc_binary(
    name = "syndicate_explorer",
    deps = [":syndicate"],
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
