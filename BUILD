cc_library(
  name = "types_hdrs",
  hdrs = glob([
    "src/types/**/*.h",
    "src/types/**/*.hpp",
  ]) + [
    "src/types/dll_entry.inc",
  ],
  includes = ["src"],
  
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

filegroup(
    name = "org_cultlang_types_import",
    srcs = ["org_cultlang_types.dll"],
    output_group = "interface_library",
)

cc_import(
  name = "types_lib",
  interface_library = ":org_cultlang_types_import",
  shared_library = "org_cultlang_types.dll",
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
