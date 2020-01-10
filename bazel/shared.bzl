# A helper macro for making cross platform shared objects

# https://docs.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=vs-2019
#   In case we have to link dlls ourselves apparently

def crossplatform_shared(
        basename,
        modulename,
        deps=[],
        module_deps=[],
        so_linkopts=[],
        dll_linkopts=[],
        dylib_linkopts=[],
    ):
    # from: https://github.com/bazelbuild/bazel/blob/master/examples/windows/dll/windows_dll_library.bzl
    
    dll_name = basename + ".dll"
    native.cc_binary(
        name = dll_name,
        deps = deps,
        linkopts = dll_linkopts,
        linkshared = True,
    )

    dll_import_lib_name = basename + "_dll_import_lib"
    dll_import_name = basename + "_dll_import"
    native.filegroup(
        name = dll_import_lib_name,
        srcs = [":" + dll_name],
        output_group = "interface_library",
    )
    native.cc_import(
        name = dll_import_name,
        interface_library = ":" + dll_import_lib_name,
        shared_library = ":" + dll_name,
    )

    dylib_name = "lib" + basename + ".dylib"
    native.cc_binary(
        name = dylib_name,
        deps = deps,
        linkopts = dylib_linkopts,
        linkshared = True,
    )

    so_name = "lib" + basename + ".so"
    native.cc_binary(
        name = so_name,
        deps = deps,
        linkopts = so_linkopts,
        linkshared = True,
    )

    native.cc_library(
        name = modulename,
        visibility = ["//visibility:public"],

        deps = module_deps + select({
            "@bazel_tools//src/conditions:windows": [dll_import_name],
            "//conditions:default": [],
        }),

        srcs = select({
            "@bazel_tools//src/conditions:windows": [],
            "@bazel_tools//src/conditions:darwin": [dylib_name],
            "//conditions:default": [":" + so_name],
        }),

        features = ["copy_dynamic_libraries_to_binary"]
    )