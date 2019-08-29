load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def cultlang_syndicate_deps():
    if "catch" not in native.existing_rules():
        http_archive(
            name = "catch",
            url = "https://github.com/cgrinker/Catch2/archive/5e6488fd9949cb41d717a72c8c4603b7e37d68cd.zip",
            sha256 = "91e3e0610572adefa301a6e55ac48ab0a3c8ff61787ce6930e346ff36e86905c",
            strip_prefix = "Catch2-5e6488fd9949cb41d717a72c8c4603b7e37d68cd",
        )

    if "fmt" not in native.existing_rules():
        http_archive(
            name = "fmt",
            urls = ["https://github.com/cgrinker/fmt/archive/f16f77297e6bb4df38d4c858edb3295f55716cb4.zip"],
            strip_prefix = "fmt-f16f77297e6bb4df38d4c858edb3295f55716cb4",
            sha256 = "edcacda20bf46be208fbc49cedee2a8a321005dd833752ccc6aa7a1a9d75dc23",
        )

    if "spdlog" not in native.existing_rules():
        http_archive(
            name = "spdlog",
            urls = ["https://github.com/cgrinker/spdlog/archive/3bf4a07dc286e10fa32d6503fded647c9ee02d26.zip"],
            strip_prefix = "spdlog-3bf4a07dc286e10fa32d6503fded647c9ee02d26",
            sha256 = "d00ccd202e2abae832441b9121330a4b8b82fdc60564edb315c89506fa3772c3",
        )

    

    if "cultlang_stdext" not in native.existing_rules():
        http_archive(
            name = "stdext",
            urls = ["https://github.com/cultlang/stdext/archive/v1.0.1.zip"],
            strip_prefix = "stdext-1.0.1",
            sha256 = "342aa6f86d12e5e01262e3b2b15a3843ff6cd19c747bab6189a20c0e66754796",
        )

    if "cultlang_graph" not in native.existing_rules():
        http_archive(
            name = "graph",
            urls = ["https://github.com/cultlang/graph/archive/v0.1.0.zip"],
            strip_prefix = "graph-0.1.0",
            sha256 = "cf30e79fbaf0aa2c82a802239927b43a6e963fac970d8273f77823c3e998f582",
        )