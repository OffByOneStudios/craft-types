# Bazel Build File For Type system


cc_library(
    name="types",
    visibility = ["//visibility:public"],
    hdrs=glob([
        "src/**/*.h*",
        "src/types/dll_entry.inc"
    ]),
    #srcs=glob([
    #    "src/**/*.c*"
    #]),
    srcs=[
        "src/syn/system/SymbolTable.cpp",
        "src/syn/system/TypeStore.cpp",
        "src/syn/system/TypeId.cpp",

        "src/syn/core/system_graph.cpp",
        "src/syn/core/cpp_graph.cpp",

        "src/syn/cpp/cpp_interface.cpp",

        "src/syn/boot/default_types_cpp.cpp",
    ],
    defines=[
        "CULTLANG_TYPES_DLL",
        "CULT_CURRENT_PACKAGE=\\\"org_cultlang_" + "types" + "\\\""
    ],
    includes=[
        "src"
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17"],
        "//conditions:default": ["-std=c++17"],
    }),
    
    deps=[
        "@graph//:graph",
        "@stdext//:stdext",
        "@spdlog//:headers",
        "@boost//:callable_traits",
    ]
)

cc_binary(
    name = "Syndicate.dll",
    deps = ["types"],
    linkopts = ["/ENTRY:syn_DLLMAIN"],
    linkshared = 1,
)

cc_binary(
    name = "libSyndicate.dylib",
    deps = ["types"],
    linkshared = 1,
)

cc_binary(
    name = "libSyndicate.so",
    deps = ["types"],
    linkshared = 1,
)

cc_binary(
    name = "syndicate_explorer",
    deps = select({
        "@bazel_tools//src/conditions:windows": ["Syndicate.dll"],
        "@bazel_tools//src/conditions:darwin": ["libSyndicate.dylib"],
        "//conditions:default": ["libSyndicate.so"],
    }),
    srcs = glob([
        "entry/*.c*",
        "entry/*.h*",
    ]),
)


cc_test(
    name = "unit",
    includes = ["test"],
    srcs = glob([
        "test/unit/**/*.cpp",
        "test/unit/**/*.h*"
    ]),
    deps = [
        "types",
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
        "types",
       "@catch//:single_include",
    ],
)
