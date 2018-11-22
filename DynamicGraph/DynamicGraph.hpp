#pragma once

#include <list>
#include <map>
#include <utility>
#include <vector>

#include "DynamicForest/DynamicForest.hpp"

namespace usp {

class DynamicGraph {
 public:
    explicit DynamicGraph(int n);
    void insert(int u, int v);
    void remove(int u, int v);
    bool is_connected(int u, int v);
 private:
    enum edge_type {
       TREE,
       NON_TREE
    };
    const int LOGN;
    std::vector<DynamicForest> F;
    std::vector<std::vector<std::list<int>>> edges[2];
    std::map<std::pair<int, int>, int> level;
    std::map<std::pair<int, int>, std::list<int>::iterator> iterator;
    void downgrade(int u, int v, int l, edge_type type);
    void downgrade_tree_edges(int u, int l);
    void insert(int u, int v, int l, edge_type type);
    void remove(int u, int v, edge_type type);
};

} // namespace usp
