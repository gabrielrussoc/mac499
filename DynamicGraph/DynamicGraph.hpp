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
    const int LOGN;
    std::vector<DynamicForest> F;
    std::vector<std::vector<std::list<int>>> tree_edges;
    std::vector<std::vector<std::list<int>>> non_tree_edges;
    std::map<std::pair<int, int>, int> level;
    std::map<std::pair<int, int>, std::list<int>::iterator> iterator;
    void downgrade_non_tree(int u, int v, int l);
    void downgrade_tree_edges(int u, int l);
    void replace(int u, int v, int l);
    void insert_tree(int u, int v, int l);
    void remove_tree(int u, int v);
    void insert_non_tree(int u, int v);
    void remove_non_tree(int u, int v);
};

} // namespace usp
