# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "DynamicForest",
    srcs = ["DynamicForest.cpp"],
    hdrs = ["DynamicForest.hpp"],
    deps = [],
    copts = [
        "-O2",
    ],
    visibility = [
        "//visibility:public",
    ]
)

cc_test(
    name = "DynamicForestTest",
    srcs = ["DynamicForestTest.cpp"],
    deps = [
        "@gtest//:gtest_main",
        ":DynamicForest",
    ],
)
