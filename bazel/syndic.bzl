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
