#include "gtest/gtest.h"
#include "NaiveDynamicGraph/NaiveDynamicGraph.hpp"

using usp::NaiveDynamicGraph;

TEST(NaiveDynamicGraph, Generic) {
  NaiveDynamicGraph dg(4);
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

TEST(NaiveDynamicGraph, HasEdge) {
  NaiveDynamicGraph dg(4);
  EXPECT_EQ(dg.has_edge(0, 2), false);
  dg.insert(0, 1);
  dg.insert(1, 2);
  EXPECT_EQ(dg.has_edge(0, 1), true);
  EXPECT_EQ(dg.has_edge(1, 2), true);
  dg.remove(0, 1);
  EXPECT_EQ(dg.has_edge(0, 1), false);
  dg.remove(1, 2);
  EXPECT_EQ(dg.has_edge(1, 2), false);
}

TEST(NaiveDynamicGraph, Grid) {
  /*
    0 1 2
    3 4 5
  */
  NaiveDynamicGraph dg = NaiveDynamicGraph::grid(2, 3);
  EXPECT_EQ(dg.has_edge(0, 1), true);
  EXPECT_EQ(dg.has_edge(1, 2), true);
  EXPECT_EQ(dg.has_edge(3, 4), true);
  EXPECT_EQ(dg.has_edge(4, 5), true);
  EXPECT_EQ(dg.has_edge(0, 3), true);
  EXPECT_EQ(dg.has_edge(1, 4), true);
  EXPECT_EQ(dg.has_edge(2, 5), true);
}
