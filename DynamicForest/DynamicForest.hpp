#pragma once

#include <map>
#include <utility>
#include <random>

namespace usp {

class DynamicForest {
 public:
    explicit DynamicForest(size_t n);
    ~DynamicForest();
    void add_edge(int u, int v);
    void remove_edge(int u, int v);
    bool is_connected(int u, int v);

 private:
    struct Node {
        explicit Node(uint64_t priority);
        uint64_t priority;
        Node* left;
        Node* right;
        Node* parent;
        size_t size;
        bool greaterThanParent;
        size_t order();
        void update_size();
        Node* get_root();
    };
    std::map<std::pair<int, int>, Node*> dict;
    std::mt19937_64 gen;
    static std::pair<Node*, Node*> split(Node *node, size_t k);
    static Node* merge(Node *a, Node *b);
    Node* reroot(int u);
    uint64_t random();
};

} // namespace usp
