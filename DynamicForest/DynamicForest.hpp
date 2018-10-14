#pragma once

#include <map>
#include <utility>
#include <random>
#include <vector>

namespace usp {

class DynamicForest {
 public:
    explicit DynamicForest(size_t n);
    ~DynamicForest();
    void link(int u, int v);
    void cut(int u, int v);
    bool is_connected(int u, int v);
    bool has_edge(int u, int v);
    size_t size(int u);
    void mark(int u);
    void unmark(int u);
    std::vector<int> all_marked(int u);

 private:
    struct Node {
        explicit Node(uint64_t priority, int label);
        uint64_t priority;
        Node* left;
        Node* right;
        Node* parent;
        size_t size;
        bool greaterThanParent;
        int mark;
        int marked_children;
        int label;
        size_t order();
        void update_size();
        void update_marked();
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
