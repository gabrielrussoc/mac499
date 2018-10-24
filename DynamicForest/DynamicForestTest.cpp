#include <vector>

#include "gtest/gtest.h"
#include "DynamicForest/DynamicForest.hpp"

using usp::DynamicForest;
using std::vector;

vector<int> sorted(vector<int> v) {
  sort(v.begin(), v.end());
  return v;
}

TEST(DynamicForest, LinkOnly) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);
  EXPECT_EQ(df.is_connected(1, 2), true);
  EXPECT_EQ(df.is_connected(3, 4), true);
  EXPECT_EQ(df.is_connected(1, 4), false);
  EXPECT_EQ(df.is_connected(2, 4), false);
  EXPECT_EQ(df.is_connected(0, 0), true);
}

TEST(DynamicForest, LinkAndCut) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);
  EXPECT_EQ(df.is_connected(1, 2), true);
  EXPECT_EQ(df.is_connected(3, 4), true);
  EXPECT_EQ(df.is_connected(1, 4), false);
  EXPECT_EQ(df.is_connected(2, 4), false);
  EXPECT_EQ(df.is_connected(0, 0), true);
  df.cut(0, 2);
  EXPECT_EQ(df.is_connected(1, 2), false);
  df.cut(3, 4);
  EXPECT_EQ(df.is_connected(3, 4), false);
  EXPECT_EQ(df.is_connected(4, 4), true);
}

TEST(DynamicForest, LinkAndCut2) {
  DynamicForest df(5);
  EXPECT_EQ(df.is_connected(0, 4), false);
  df.link(0, 1);
  df.link(0, 2);
  df.link(2, 3);
  df.link(4, 3);
  EXPECT_EQ(df.is_connected(0, 4), true);
  df.cut(3, 4);
  EXPECT_EQ(df.is_connected(0, 4), false);
  df.cut(2, 3);
  df.link(2, 4);
  EXPECT_EQ(df.is_connected(0, 4), true);
}

TEST(DynamicForest, MarkWhite) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);

  df.mark_white(1);
  df.mark_white(2);
  vector<int> expected = {1, 2};
  EXPECT_EQ(sorted(df.white_nodes(0)), expected);
  EXPECT_EQ(sorted(df.white_nodes(1)), expected);
  EXPECT_EQ(sorted(df.white_nodes(2)), expected);

  df.mark_white(4);
  expected = {4};
  EXPECT_EQ(sorted(df.white_nodes(3)), expected);
  EXPECT_EQ(sorted(df.white_nodes(4)), expected);
}

TEST(DynamicForest, UnmarkWhite) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);

  df.mark_white(1);
  df.mark_white(2);
  vector<int> expected = {1, 2};
  EXPECT_EQ(sorted(df.white_nodes(0)), expected);
  EXPECT_EQ(sorted(df.white_nodes(1)), expected);
  EXPECT_EQ(sorted(df.white_nodes(2)), expected);

  df.unmark_white(1);
  expected = {2};
  EXPECT_EQ(sorted(df.white_nodes(0)), expected);
  EXPECT_EQ(sorted(df.white_nodes(1)), expected);
  EXPECT_EQ(sorted(df.white_nodes(2)), expected);

  df.mark_white(4);
  expected = {4};
  EXPECT_EQ(df.white_nodes(3), expected);
  EXPECT_EQ(df.white_nodes(4), expected);

  df.unmark_white(4);
  expected = {};
  EXPECT_EQ(df.white_nodes(3), expected);
  EXPECT_EQ(df.white_nodes(4), expected);
}

TEST(DynamicForest, MarkBlack) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);

  df.mark_black(1);
  df.mark_black(2);
  vector<int> expected = {1, 2};
  EXPECT_EQ(sorted(df.black_nodes(0)), expected);
  EXPECT_EQ(sorted(df.black_nodes(1)), expected);
  EXPECT_EQ(sorted(df.black_nodes(2)), expected);

  df.mark_black(4);
  expected = {4};
  EXPECT_EQ(sorted(df.black_nodes(3)), expected);
  EXPECT_EQ(sorted(df.black_nodes(4)), expected);
}

TEST(DynamicForest, UnmarkBlack) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);

  df.mark_black(1);
  df.mark_black(2);
  vector<int> expected = {1, 2};
  EXPECT_EQ(sorted(df.black_nodes(0)), expected);
  EXPECT_EQ(sorted(df.black_nodes(1)), expected);
  EXPECT_EQ(sorted(df.black_nodes(2)), expected);

  df.unmark_black(1);
  expected = {2};
  EXPECT_EQ(sorted(df.black_nodes(0)), expected);
  EXPECT_EQ(sorted(df.black_nodes(1)), expected);
  EXPECT_EQ(sorted(df.black_nodes(2)), expected);

  df.mark_black(4);
  expected = {4};
  EXPECT_EQ(df.black_nodes(3), expected);
  EXPECT_EQ(df.black_nodes(4), expected);

  df.unmark_black(4);
  expected = {};
  EXPECT_EQ(df.black_nodes(3), expected);
  EXPECT_EQ(df.black_nodes(4), expected);
}

// TODO(gabrielrc): testes para pretos e brancos

TEST(DynamicForest, Size) {
  DynamicForest df(5);
  df.link(0, 1);
  df.link(0, 2);
  df.link(3, 4);
  EXPECT_EQ(df.size(0), 3);
  EXPECT_EQ(df.size(1), 3);
  EXPECT_EQ(df.size(2), 3);
  df.cut(0, 2);
  EXPECT_EQ(df.size(0), 2);
  EXPECT_EQ(df.size(1), 2);
  EXPECT_EQ(df.size(2), 1);
  df.cut(3, 4);
  EXPECT_EQ(df.size(3), 1);
  EXPECT_EQ(df.size(4), 1);
}
