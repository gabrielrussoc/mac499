#include "DynamicForest/DynamicForest.hpp"

using std::pair;

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
        node->parent = NULL; // pensar mais nisso aqui, precisa?
        node->update_size();
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
        return b;
    } else {
        Node *node = merge(a->right, b);
        a->right = node;
        node->parent = a;
        node->greaterThanParent = true;
        a->update_size();
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

DynamicForest::Node::Node(uint64_t priority) :
    priority(priority),
    left(NULL),
    right(NULL),
    parent(NULL),
    size(1),
    greaterThanParent(false) {}

DynamicForest::DynamicForest(size_t n) : gen((std::random_device())()) {
    for (int u = 0; u < n; u++) {
        this->dict[{u, u}] = new Node(random());
    }
}

const char kRight = 0;
const char kLeft = 1;

DynamicForest::Node* DynamicForest::shift(int u, char dir) {
    DynamicForest::Node *uu = this->dict[{u, u}];
    size_t k = uu->order();
    DynamicForest::Node *root = uu->get_root();
    auto spl = split(root, k - dir);
    return merge(spl.second, spl.first);
}

void DynamicForest::add_edge(int u, int v) {
    Node *uv = new Node(random());
    this->dict[{u, v}] = uv;
    Node *vu = new Node(random());
    this->dict[{v, u}] = vu;
    Node *nu = shift(u, kRight);
    Node *nv = shift(v, kLeft);
    merge(nu, merge(uv, merge(nv, vu)));
}

bool DynamicForest::is_connected(int u, int v) {
    return this->dict[{u, u}]->get_root() == this->dict[{v, v}]->get_root();
}

void DynamicForest::remove_edge(int u, int v) {
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
