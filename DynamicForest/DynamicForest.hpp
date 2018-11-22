#pragma once

#include <map>
#include <memory>
#include <utility>
#include <random>
#include <vector>

namespace usp {

class DynamicForest {
 public:
    explicit DynamicForest(size_t n);
    DynamicForest(const DynamicForest&);
    void link(int u, int v);
    void cut(int u, int v);
    bool is_connected(int u, int v);
    bool has_edge(int u, int v);
    size_t size(int u);
    void mark_white(int u);
    void unmark_white(int u);
    void mark_black(int u);
    void unmark_black(int u);
    std::vector<int> white_nodes(int u);
    std::vector<int> black_nodes(int u);

 private:
    struct Node {
        explicit Node(uint64_t priority, int label);
        uint64_t priority;
        Node* left;
        Node* right;
        Node* parent;
        size_t size;
        bool greaterThanParent;
        int white_count;
        int black_count;
        int white_children;
        int black_children;
        int label;
        size_t order();
        void update_size();
        void update_marks();
        Node* get_root();
    };
    std::map<std::pair<int, int>, std::unique_ptr<Node>> dict;
    std::mt19937_64 gen;
    static std::pair<Node*, Node*> split(Node *node, size_t k);
    static Node* merge(Node *a, Node *b);
    Node* reroot(int u);
    uint64_t random();
};

} // namespace usp
