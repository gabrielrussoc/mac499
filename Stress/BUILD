# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_test
cc_test(
    name = "StressTest",
    srcs = ["StressTest.cpp"],
    deps = [
        "@gtest//:gtest_main",
        "//NaiveDynamicGraph:NaiveDynamicGraph",
        "//DynamicGraph:DynamicGraph",
    ],
)