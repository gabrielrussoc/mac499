# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "DynamicGraph",
    srcs = ["DynamicGraph.cpp"],
    hdrs = ["DynamicGraph.hpp"],
    deps = [
        "//DynamicForest:DynamicForest",
    ],
    visibility = [
        "//visibility:public",
    ],
)

cc_test(
    name = "DynamicGraphTest",
    srcs = ["DynamicGraphTest.cpp"],
    deps = [
        "@gtest//:gtest_main",
        ":DynamicGraph",
    ],
)