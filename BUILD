# Bazel Build File For Type system

load("//build_tools:macros.bzl", "header_generator", "dll_generator", "entrypoint_generator")

header_generator(  
  extra_headers = ["src/types/dll_entry.inc",],
  deps = ["@spdlog//:spdlog", "@boost//:callable_traits"]
)

dll_generator()

entrypoint_generator(name = "runtime_explorer")

cc_test(
    name = "tests",
    srcs = glob(["test/unit/**/*.cpp"]),
    deps = [
        "types",
        "@catch//:catch",
    ],
    copts = select({
        "@bazel_tools//src/conditions:windows": ["/std:c++17"],
        "@bazel_tools//src/conditions:darwin": ["-std=c++17"],
        "//conditions:default": ["-std=c++17"],
    }),
)

cc_test(
    name = "literate_tests",
    srcs = glob(["test/literate/*.cpp"]),
    deps = [
        "types",
        "@catch//:catch",
    ],
)