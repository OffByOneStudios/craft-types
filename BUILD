load("@cultlang_syndicate//:bazel/syndic.bzl", "syndic")

syndic(
    basename = "Syndicate",
    
    hdrs = glob([
        "src/**/*.h*",
        "src/syn/dll_entry.inc"
    ]),
    includes = [
        "src"
    ],

    dll_define = "CULTLANG_SYNDICATE_DLL",
    # todo: use glob again
    srcs = [
        "src/syn/system/SymbolTable.cpp",
        "src/syn/system/TypeStore.cpp",
        "src/syn/system/TypeId.cpp",
        "src/syn/system/ModuleBase.cpp",

        "src/syn/core/system_graph.cpp",
        "src/syn/core/module_graph.cpp",
        "src/syn/core/syndicate_graph.cpp",
        "src/syn/core/cpp_graph.cpp",
        "src/syn/core/containers.cpp",

        "src/syn/cpp/cpp_interface.cpp",

        "src/syn/boot/default_types_cpp.cpp",

        "src/syn/syn.cpp",
    ],

    headers_deps = [
        "@spdlog//:headers",
        "@boost//:callable_traits",
        "@cultlang_stdext//:headers",
        "@cultlang_graph//:graph",
    ],
    code_deps = [
        "@cultlang_stdext//:stdext",
    ],

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
