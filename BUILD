# Bazel Build File For Type system


cc_library(
    name="types",
    visibility = ["//visibility:public"],
    hdrs=glob([
        "src/**/*.h*",
        "src/types/dll_entry.inc"
    ]),
    srcs=glob([
        "src/**/*.c*"
    ]),
    defines=[
        "CULTLANG_TYPES_DLL",
        "CULT_CURRENT_PACKAGE=\\\"org_cultlang_" + "types" + "\\\""
    ],
    includes=[
        "src"
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17"],
        "@bazel_tools//src/conditions:darwin": ["-std=c++17"],
        "//conditions:default": ["-std=c++17"],
    }),
    deps=[
        "@graph//:graph",
        "@stdext//:stdext",
        "@spdlog//:headers",
        "@boost//:callable_traits",
    ]
)
