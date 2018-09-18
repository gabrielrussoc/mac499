#include "gtest/gtest.h"
#include "DynamicForest/DynamicForest.hpp"

using usp::DynamicForest;

TEST(DynamicForest, AddOnly) {
  DynamicForest df(5);
  df.add_edge(0, 1);
  df.add_edge(0, 2);
  df.add_edge(3, 4);
  EXPECT_EQ(df.is_connected(1, 2), true);
  EXPECT_EQ(df.is_connected(3, 4), true);
  EXPECT_EQ(df.is_connected(1, 4), false);
  EXPECT_EQ(df.is_connected(2, 4), false);
  EXPECT_EQ(df.is_connected(0, 0), true);
}

TEST(DynamicForest, AddAndRemove) {
  DynamicForest df(5);
  df.add_edge(0, 1);
  df.add_edge(0, 2);
  df.add_edge(3, 4);
  EXPECT_EQ(df.is_connected(1, 2), true);
  EXPECT_EQ(df.is_connected(3, 4), true);
  EXPECT_EQ(df.is_connected(1, 4), false);
  EXPECT_EQ(df.is_connected(2, 4), false);
  EXPECT_EQ(df.is_connected(0, 0), true);
  df.remove_edge(0, 2);
  EXPECT_EQ(df.is_connected(1, 2), false);
  df.remove_edge(3, 4);
  EXPECT_EQ(df.is_connected(3, 4), false);
  EXPECT_EQ(df.is_connected(4, 4), true);
}

TEST(DynamicForest, AddAndRemove2) {
  DynamicForest df(5);
  EXPECT_EQ(df.is_connected(0, 4), false);
  df.add_edge(0, 1);
  df.add_edge(0, 2);
  df.add_edge(2, 3);
  df.add_edge(4, 3);
  EXPECT_EQ(df.is_connected(0, 4), true);
  df.remove_edge(3, 4);
  EXPECT_EQ(df.is_connected(0, 4), false);
  df.remove_edge(2, 3);
  df.add_edge(2, 4);
  EXPECT_EQ(df.is_connected(0, 4), true);
}