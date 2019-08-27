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
        # TODO: use local defines
        copts = select({
            "@bazel_tools//src/conditions:windows": ["/MD", "/std:c++17", "/D" + dll_define],
            "//conditions:default": ["-std=c++17", "-D" + dll_define],
        }),
        linkopts = code_linkopts,

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
            # TODO: use local defines
            copts = select({
                "@bazel_tools//src/conditions:windows": ["/MD", "/std:c++17", "/DCULTLANG_SYNDICATE_NODLL"],
                "//conditions:default": ["-std=c++17", "-DCULTLANG_SYNDICATE_NODLL"],
            }),
            linkopts = code_linkopts,
        )
