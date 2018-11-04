#include <cstdlib>
#include <utility>
#include <vector>

#include "gtest/gtest.h"
#include "DynamicGraph/DynamicGraph.hpp"
#include "NaiveDynamicGraph/NaiveDynamicGraph.hpp"

using std::pair;
using std::vector;
using usp::NaiveDynamicGraph;
using usp::DynamicGraph;

const int kIterations = 10000;
const int kCheck = 100;

void check_connectivity(NaiveDynamicGraph &naive, DynamicGraph &dg) {
    size_t n = naive.size();
    for (int u = 0; u < n; u++) {
        for (int v = u; v < n; v++) {
            // EXPECT_EQ(naive.is_connected(u, v), dg.is_connected(u, v));
            assert(naive.is_connected(u, v) == dg.is_connected(u, v));
        }
    }
}

TEST(Stress, Grid100) {
    int n = 10;
    int m = 10;
    NaiveDynamicGraph original = NaiveDynamicGraph::grid(n, m;
    NaiveDynamicGraph naive(n * m);
    DynamicGraph dg(n * m);

    vector<pair<int, int>> edges = original.edges();
    srand(7);
    for (int iter = 1; iter <= kIterations; iter++) {
        fprintf(stderr, "iter %d\n", iter);
        int idx = rand() % edges.size();
        int u = edges[idx].first;
        int v = edges[idx].second;
        if (naive.has_edge(u, v)) {
            fprintf(stderr, "Remove %d-%d\n", u, v);
            naive.remove(u, v);
            dg.remove(u, v);
        } else {
            fprintf(stderr, "Adiciona %d-%d\n", u, v);
            naive.insert(u, v);
            dg.insert(u, v);
        }
        if (iter % kCheck == 0) {
            check_connectivity(naive, dg);
        }
    }
}