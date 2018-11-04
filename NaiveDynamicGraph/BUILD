# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "NaiveDynamicGraph",
    srcs = ["NaiveDynamicGraph.cpp"],
    hdrs = ["NaiveDynamicGraph.hpp"],
    visibility = [
        "//visibility:public",
    ],
    deps = [],
)

cc_test(
    name = "NaiveDynamicGraphTest",
    srcs = ["NaiveDynamicGraphTest.cpp"],
    deps = [
        "@gtest//:gtest_main",
        ":NaiveDynamicGraph",
    ],
)