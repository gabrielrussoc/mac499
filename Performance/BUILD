# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "Performance",
    srcs = ["Performance.cpp"],
    copts = [
        "-O2",
    ],
    deps = [
        "//DynamicGraph:DynamicGraph",
        "//NaiveDynamicGraph:NaiveDynamicGraph"
    ],
)
