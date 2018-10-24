#include "DynamicForest/DynamicForest.hpp"

#include <queue>

using std::pair;
using std::vector;
using std::queue;

namespace usp {

// Devolve a ordem de um nó. 1-indexed.
size_t DynamicForest::Node::order() {
    size_t qt = 0;
    bool lessOrEqual = true;
    DynamicForest::Node *node = this;
    while (node != NULL) {
        if (lessOrEqual) {
            qt++;
            if (node->left != NULL) {
                qt += node->left->size;
            }
        }
        lessOrEqual = node->greaterThanParent;
        node = node->parent;
    }
    return qt;
}

void DynamicForest::Node::update_size() {
    this->size = 1;
    if (this->left != NULL) {
        this->size += this->left->size;
    }
    if (this->right != NULL) {
        this->size += this->right->size;
    }
}
void DynamicForest::Node::update_marks() {
    this->white_children = this->white_count;
    this->black_children = this->black_count;
    if (this->left != NULL) {
        this->white_children += this->left->white_children;
        this->black_children += this->left->black_children;
    }
    if (this->right != NULL) {
        this->white_children += this->right->white_children;
        this->black_children += this->right->black_children;
    }
}

// Devolve duas árvores, com os k-primeiros elementos na primeira e o resto na segunda
pair<DynamicForest::Node*, DynamicForest::Node*> DynamicForest::split(Node *node, size_t k) {
    if (node == NULL) {
        return {NULL, NULL};
    }
    size_t less_than = 0;
    if (node->left != NULL) {
        less_than = node->left->size;
    }
    if (less_than < k) {
        auto spl = split(node->right, k - less_than - 1);
        Node *left = spl.first;
        Node *right = spl.second;
        node->right = left;
        if (left != NULL) {
            left->parent = node;
            left->greaterThanParent = true;
        }
        node->parent = NULL;
        node->update_size();
        node->update_marks();
        return {node, right};
    } else {
        auto spl = split(node->left, k);
        Node *left = spl.first;
        Node *right = spl.second;
        node->left = right;
        if (right != NULL) {
            right->parent = node;
            right->greaterThanParent = false;
        }
        node->parent = NULL;
        node->update_size();
        node->update_marks();
        return {left, node};
    }
}

DynamicForest::Node* DynamicForest::merge(DynamicForest::Node *a, DynamicForest::Node *b) {
    if (a == NULL) {
        return b;
    }
    if (b == NULL) {
        return a;
    }
    if (a->priority < b->priority) {
        Node *node = merge(a, b->left);
        b->left = node;
        node->parent = b;
        node->greaterThanParent = false;
        b->update_size();
        b->update_marks();
        return b;
    } else {
        Node *node = merge(a->right, b);
        a->right = node;
        node->parent = a;
        node->greaterThanParent = true;
        a->update_size();
        a->update_marks();
        return a;
    }
}

DynamicForest::Node* DynamicForest::Node::get_root() {
    DynamicForest::Node* node = this;
    while (node->parent != NULL) {
        node = node->parent;
    }
    return node;
}

DynamicForest::Node::Node(uint64_t priority, int label = -1) :
    priority(priority),
    left(NULL),
    right(NULL),
    parent(NULL),
    size(1),
    greaterThanParent(false),
    white_count(0),
    black_count(0),
    white_children(0),
    black_children(0),
    label(label) {}

DynamicForest::DynamicForest(size_t n) : gen((std::random_device())()) {
    for (int u = 0; u < n; u++) {
        this->dict[{u, u}] = new Node(random(), u);
    }
}

DynamicForest::Node* DynamicForest::reroot(int u) {
    DynamicForest::Node *uu = this->dict[{u, u}];
    size_t k = uu->order();
    DynamicForest::Node *root = uu->get_root();
    auto spl = split(root, k - 1);
    return merge(spl.second, spl.first);
}

void DynamicForest::link(int u, int v) {
    Node *uv = new Node(random());
    this->dict[{u, v}] = uv;
    Node *vu = new Node(random());
    this->dict[{v, u}] = vu;
    Node *nu = reroot(u);
    Node *nv = reroot(v);
    merge(nu, merge(uv, merge(nv, vu)));
}

bool DynamicForest::is_connected(int u, int v) {
    return this->dict[{u, u}]->get_root() == this->dict[{v, v}]->get_root();
}

bool DynamicForest::has_edge(int u, int v) {
    return this->dict.find({u, v}) != this->dict.end();
}

void DynamicForest::cut(int u, int v) {
    auto it_uv = this->dict.find({u, v});
    auto it_vu = this->dict.find({v, u});
    Node *uv = it_uv->second;
    Node *vu = it_vu->second;
    this->dict.erase(it_uv);
    this->dict.erase(it_vu);
    Node *root = uv->get_root();
    size_t left = uv->order();
    size_t right = vu->order();
    if (left > right) {
        std::swap(left, right);
    }
    auto tmp = split(root, right);
    Node *ur = tmp.second;
    tmp = split(tmp.first, left - 1);
    Node *ul = tmp.first;
    Node *vm = tmp.second;
    merge(ul, ur);

    tmp = split(vm, vm->size - 1);
    tmp = split(tmp.first, 1);
    delete uv;
    delete vu;
}

size_t DynamicForest::size(int u) {
    int sz = dict[{u, u}]->get_root()->size;
    return (sz + 2) / 3;
}

void DynamicForest::mark_white(int u) {
    Node *node = dict[{u, u}];
    node->white_count++;
    while (node != NULL) {
        node->white_children++;
        node = node->parent;
    }
}

void DynamicForest::mark_black(int u) {
    Node *node = dict[{u, u}];
    node->black_count++;
    while (node != NULL) {
        node->black_children++;
        node = node->parent;
    }
}

void DynamicForest::unmark_white(int u) {
    Node *node = dict[{u, u}];
    node->white_count--;
    while (node != NULL) {
        node->white_children--;
        node = node->parent;
    }
}

void DynamicForest::unmark_black(int u) {
    Node *node = dict[{u, u}];
    node->black_count--;
    while (node != NULL) {
        node->black_children--;
        node = node->parent;
    }
}

vector<int> DynamicForest::white_nodes(int u) {
    vector<int> all;
    Node *root = dict[{u, u}]->get_root();
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        if (node->white_count > 0) {
            all.push_back(node->label);
        }
        if (node->white_children > 0) {
            if (node->left != NULL) {
                q.push(node->left);
            }
            if (node->right != NULL) {
                q.push(node->right);
            }
        }
    }
    return all;
}

vector<int> DynamicForest::black_nodes(int u) {
    vector<int> all;
    Node *root = dict[{u, u}]->get_root();
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        if (node->black_count > 0) {
            all.push_back(node->label);
        }
        if (node->black_children > 0) {
            if (node->left != NULL) {
                q.push(node->left);
            }
            if (node->right != NULL) {
                q.push(node->right);
            }
        }
    }
    return all;
}

DynamicForest::~DynamicForest() {
    for (auto kv : dict) {
        delete kv.second;
    }
}

uint64_t DynamicForest::random() {
    std::uniform_int_distribution<uint64_t> distr;
    return distr(this->gen);
}

} // namespace usp
