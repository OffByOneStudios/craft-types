load("@cultlang_syndicate//:bazel/syndic.bzl", "syndic")

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

    headers_deps = [
        "@graph//:graph",
        "@spdlog//:headers",
        "@stdext//:headers",
        "@boost//:callable_traits",
    ],
    code_deps = [
        "@stdext//:stdext",
    ],

    headers_linkopts = select({
        "@bazel_tools//src/conditions:windows": [
        ],
        "//conditions:default": [
            "-ldl",
            "-lpthread",
            "-lstdc++fs",
        ],
    }),
)

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
