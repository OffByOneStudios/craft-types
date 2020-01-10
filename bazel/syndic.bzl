load("@cultlang_syndicate//:bazel/shared.bzl", "crossplatform_shared")

def syndic(
        basename,

        dll_define,

        codename="code",
        headersname="headers",
        modulename="module",

        headers_deps=[],
        code_deps=[],

        headers_linkopts=[],
        code_linkopts=[],

        includes=[],
        srcs=[],
        hdrs=[],

        # if empty, does not add
        supportname = "support",
    ):

    native.cc_library(
        name = headersname,
        deps = headers_deps,

        includes = includes,
        hdrs = hdrs,

        linkopts = headers_linkopts,
    )
    native.cc_library(
        name = codename,
        deps = [":" + headersname] + code_deps,

        srcs = srcs,
        local_defines = [
            dll_define,
            "CULT_CURRENT_PACKAGE=\\\"" + basename + "\\\""
        ],
        copts = select({
            "@bazel_tools//src/conditions:windows": ["/std:c++17"],
            "//conditions:default": ["-std=c++17"],
        }),
        linkopts = code_linkopts,
        features = [],

        alwayslink = True,
    )

    crossplatform_shared(
        basename = basename,
        modulename = modulename,
        deps = [":" + codename],
        module_deps = [":" + headersname],
        dll_linkopts = ["/ENTRY:syn_DLLMAIN"],
    )

    if supportname != None:
        native.cc_library(
            name = supportname,
            visibility = ["//visibility:public"],
            deps = [":" + headersname] + code_deps,

            srcs = srcs,
            local_defines = ["CULTLANG_SYNDICATE_NODLL"],
            copts = select({
                "@bazel_tools//src/conditions:windows": ["/std:c++17"],
                "//conditions:default": ["-std=c++17"],
            }),
            linkopts = code_linkopts,
            features = [],
        )
