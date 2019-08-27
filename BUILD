load("@cultlang_syndicate//:bazel/syndic.bzl", "syndic")

# todo: use glob
SRCS = [
    "src/syn/system/SymbolTable.cpp",
    "src/syn/system/TypeStore.cpp",
    "src/syn/system/TypeId.cpp",

    "src/syn/core/system_graph.cpp",
    "src/syn/core/cpp_graph.cpp",

    "src/syn/cpp/cpp_interface.cpp",

    "src/syn/boot/default_types_cpp.cpp",

    "src/syn/syn.cpp",
]
CODE_DEPS = [
    "@stdext//:stdext",
]

syndic(
    basename = "Syndicate",
    
    hdrs = glob([
        "src/**/*.h*",
        "src/types/dll_entry.inc"
    ]),
    includes = [
        "src"
    ],

    dll_define = "CULTLANG_SYNDICATE_DLL",
    srcs = SRCS,

    headers_deps = [
        "@graph//:graph",
        "@spdlog//:headers",
        "@stdext//:headers",
        "@boost//:callable_traits",
    ],
    code_deps = CODE_DEPS,

    headers_linkopts = select({
        "@bazel_tools//src/conditions:windows": [],
        "@bazel_tools//src/conditions:darwin": [],
        "//conditions:default": [
            "-ldl",
            "-lpthread",
            "-lstdc++fs",
        ],
    }),
)

# this is a special code library for using syndicate without using DLLs
# it is basically a copy of srcs, with different code
cc_library(
    name = "support",
    visibility = ["//visibility:public"],
    deps = [":headers"] + CODE_DEPS, # use the syndic defined headers library
    srcs = SRCS,
    # TODO: use local defines
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/MD", "/std:c++17", "/DCULTLANG_SYNDICATE_NODLL"],
        "//conditions:default": ["-std=c++17", "-DCULTLANG_SYNDICATE_NODLL"],
    }),
)

# utilities and tests:
cc_binary(
    name = "syndicate_explorer",
    deps = [":module"],
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
        ":module",
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
        ":module",
       "@catch//:single_include",
    ],
)
