# Bazel Build File For Type system

load("//build_tools:macros.bzl", "header_generator", "dll_generator", "entrypoint_generator")

header_generator(  
  extra_headers = ["src/types/dll_entry.inc",],
  deps = ["@boost//:callable_traits", "//util",]
)

dll_generator()

entrypoint_generator(name = "runtime_explorer")
