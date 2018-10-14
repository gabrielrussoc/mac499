#include <vector>

#include "gtest/gtest.h"
#include "DynamicGraph/DynamicGraph.hpp"

using usp::DynamicGraph;

TEST(DynamicGraph, ReplacementIsFound) {
  DynamicGraph dg(3);
  dg.insert(0, 1);
  dg.insert(1, 2);
  dg.insert(2, 0);
  EXPECT_EQ(dg.is_connected(0, 1), true);
  dg.remove(0, 1);
  EXPECT_EQ(dg.is_connected(0, 1), true);
}

TEST(DynamicGraph, Generic) {
  DynamicGraph dg(4);
  dg.insert(0, 1);
  dg.insert(1, 2);
  dg.insert(2, 3);
  dg.insert(0, 3);
  EXPECT_EQ(dg.is_connected(3, 1), true);
  dg.remove(0, 1);
  EXPECT_EQ(dg.is_connected(1, 3), true);
  dg.remove(2, 3);
  EXPECT_EQ(dg.is_connected(3, 1), false);
  dg.insert(1, 3);
  EXPECT_EQ(dg.is_connected(3, 1), true);
}
