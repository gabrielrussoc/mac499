#pragma once

#include <utility>
#include <vector>

namespace usp {

class NaiveDynamicGraph {
 public:
    explicit NaiveDynamicGraph(size_t n);
    void insert(int u, int v);
    void remove(int u, int v);
    bool is_connected(int u, int v) const;
    bool has_edge(int u, int v) const;
    size_t size() const;
    std::vector<std::pair<int, int>> edges() const;
    static NaiveDynamicGraph grid(int n, int m);
 private:
    const size_t n;
    std::vector<std::vector<bool>> adj;
};

} // namespace usp
