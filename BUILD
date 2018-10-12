cc_library(
  name = "types_hdrs",
  hdrs = glob([
    "src/types/**/*.h",
    "src/types/**/*.hpp",
  ]) + [
    "src/types/dll_entry.inc",
  ],
  includes = ["src"],
  visibility = ["//visibility:public"],
  deps = ["@boost//:callable_traits", "//util"]
)

cc_binary(
    name = "org_cultlang_types.dll",
    linkshared = 1,
    srcs = glob([
      "src/types/**/*.cpp",
      "src/types/**/*.h",
      "src/types/**/*.hpp",
    ]),
    
    deps = ["types_hdrs"],
    
    copts = ["/std:c++latest"],
    defines = ["CULTLANG_TYPES_DLL", "CULT_CURRENT_PACKAGE=\\\"org_cultlang_types\\\""],
)

genrule(
  visibility = ["//visibility:public"],
  name = "types_importlib",
  outs = ["org_cultlang_types.lib"],
  srcs = ["org_cultlang_types.dll"],
  cmd = "cp ./$(location org_cultlang_types.dll).if.lib \"$@\"",
)

cc_import(
  name = "types_lib",
  interface_library = "org_cultlang_types.lib",
  visibility = ["//visibility:public"],
  shared_library = "org_cultlang_types.dll",
)

cc_library(
    visibility = ["//visibility:public"],
    name = "org_cultlang_type_import",
    srcs = ["org_cultlang_types.lib"],
)

cc_library(
    name = "types",
    includes = ["src"],
    deps = ["types_hdrs", "types_lib"],
    visibility = ["//visibility:public"], 
)


cc_binary(
    name = "runtime_explorer",
    srcs = glob(["entry/**/*"]),
    
    deps = ["//types"],
    
    copts = ["/std:c++latest"]
)
