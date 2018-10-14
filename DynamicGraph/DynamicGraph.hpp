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
    ~DynamicGraph();
    void insert(int u, int v);
    void remove(int u, int v);
    bool is_connected(int u, int v);
 private:
    const int LOGN;
    std::vector<DynamicForest> F;
    std::vector<std::vector<std::list<int>>> adj;
    std::map<std::pair<int, int>, int> level;
    std::map<std::pair<int, int>, std::list<int>::iterator> iterator;
    void downgrade(int u, int v, int l);
    void replace(int u, int v, int l);
};

} // namespace usp
