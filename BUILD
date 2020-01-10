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
    srcs = glob([
        "src/syn/**/*.c*",
    ]),

    headers_deps = [
        "@spdlog//:headers",
        "@boost//:callable_traits",
        "@cultlang_stdext//:headers",
        "@cultlang_graph//:graph",
    ],
    code_deps = [
        "@cultlang_stdext//:code",
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
