workspace(name = "cultlang_syndicate")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

local_repository(
    name="cultlang_graph",
    path="../graph"
)

########################
# BOOST:

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "6d6fd834281cb8f8e758dd9ad76df86304bf1869",
    shallow_since = "1543903644 -0800",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

load("@cultlang_syndicate//:bazel/deps.bzl", "cultlang_syndicate_deps")
cultlang_syndicate_deps()
