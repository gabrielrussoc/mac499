#include "DynamicForest/DynamicForest.hpp"

#include <unordered_map>
#include <queue>

using std::pair;
using std::vector;
using std::queue;

namespace usp {

// Inicializa a floresta com n vertices e nenhuma aresta
DynamicForest::DynamicForest(size_t n) : gen((std::random_device())()) {
    for (int u = 0; u < n; u++) {
        this->dict[{u, u}] = std::make_unique<Node>(random(), u);
    }
}

// Copy constructor
DynamicForest::DynamicForest(const DynamicForest& other) {
    std::unordered_map<Node*, Node*> copy;
    copy[NULL] = NULL;
    for (const auto& kv : other.dict) {
        Node *orig = kv.second.get();
        copy[orig] = (dict[kv.first] = std::make_unique<Node>(*orig)).get();
    }
    for (const auto& kv : dict) {
        Node *node = kv.second.get();
        node->parent = copy[node->parent];
        node->left = copy[node->left];
        node->right = copy[node->right];
    }
}

// Adiciona a aresta uv na floresta. Supoe que uv nao existe
void DynamicForest::link(int u, int v) {
    Node *uv = (this->dict[{u, v}] = std::make_unique<Node>(random())).get();
    Node *vu = (this->dict[{v, u}] = std::make_unique<Node>(random())).get();
    Node *nu = reroot(u);
    Node *nv = reroot(v);
    merge(nu, merge(uv, merge(nv, vu)));
}

// Remove a aresta uv da floresta. Supoe que uv existe
void DynamicForest::cut(int u, int v) {
    Node *uv = dict[{u, v}].get();
    Node *vu = dict[{v, u}].get();
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
    dict.erase({u, v});
    dict.erase({v, u});
}

// Devolve true se existe um caminho entre os vertices u e v na floresta
bool DynamicForest::is_connected(int u, int v) {
    return this->dict[{u, u}]->get_root() == this->dict[{v, v}]->get_root();
}

// Devolve true se a aresta uv esta na floresta
bool DynamicForest::has_edge(int u, int v) {
    return this->dict.find({u, v}) != this->dict.end();
}

// Devolve o tamanho (numero de vertices) da componente do vertice u
size_t DynamicForest::size(int u) {
    int sz = dict[{u, u}]->get_root()->size;
    return (sz + 2) / 3;
}

// Adiciona uma marca branca no vertice u
void DynamicForest::mark_white(int u) {
    mark(u, WHITE, 1);
}

// Adiciona uma marca preta no vertice u
void DynamicForest::mark_black(int u) {
    mark(u, BLACK, 1);
}

// Remove uma marca branca do vertice u
void DynamicForest::unmark_white(int u) {
    mark(u, WHITE, -1);
}

// Remove uma marca preta do vertice u
void DynamicForest::unmark_black(int u) {
    mark(u, BLACK, -1);
}

// Devolve todos os vertices brancos na componente do vertice u
vector<int> DynamicForest::white_nodes(int u) {
    return get_nodes(u, WHITE);
}

// Devolve todos os vertices pretos na componente do vertice u
vector<int> DynamicForest::black_nodes(int u) {
    return get_nodes(u, BLACK);
}

// Adiciona delta marcas da cor color no vertice u
void DynamicForest::mark(int u, COLOR color, int delta) {
    Node *node = dict[{u, u}].get();
    node->marks[color] += delta;
    while (node != NULL) {
        node->subtree_marks[color] += delta;
        node = node->parent;
    }
}

// Devolve todos os vertices da cor color na componente do vertice u
vector<int> DynamicForest::get_nodes(int u, COLOR color) {
    vector<int> all;
    Node *root = dict[{u, u}]->get_root();
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        if (node->marks[color] > 0) {
            all.push_back(node->label);
        }
        if (node->subtree_marks[color] > 0) {
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

// Devolve um inteiro (pseudo-)aleatorio de 64 bits
uint64_t DynamicForest::random() {
    std::uniform_int_distribution<uint64_t> distr;
    return distr(this->gen);
}

// Inicializa uma BST (treap) com apenas um nó
DynamicForest::Node::Node(uint64_t priority, int label = -1) :
    priority(priority),
    left(NULL),
    right(NULL),
    parent(NULL),
    size(1),
    greaterThanParent(false),
    marks(),
    subtree_marks(),
    label(label) {}

// Devolve a ordem de um nó na BST. Indexado de 1.
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

// Atualiza as informacoes agregadas de um nó com base em seus filhos
void DynamicForest::Node::refresh() {
    this->size = 1;
    this->subtree_marks[WHITE] = this->marks[WHITE];
    this->subtree_marks[BLACK] = this->marks[BLACK];
    if (this->left != NULL) {
        this->size += this->left->size;
        this->subtree_marks[WHITE] += this->left->subtree_marks[WHITE];
        this->subtree_marks[BLACK] += this->left->subtree_marks[BLACK];
    }
    if (this->right != NULL) {
        this->size += this->right->size;
        this->subtree_marks[WHITE] += this->right->subtree_marks[WHITE];
        this->subtree_marks[BLACK] += this->right->subtree_marks[BLACK];
    }
}

// Devolve duas árvores, com os k-primeiros elementos
// da BST node na primeira e o resto na segunda
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
        node->refresh();
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
        node->refresh();
        return {left, node};
    }
}

// Une as BSTs a e b numa única árvore. Supoe que todas
// as chaves de a sao menores que as chaves de b.
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
        b->refresh();
        node->parent = b;
        node->greaterThanParent = false;
        return b;
    } else {
        Node *node = merge(a->right, b);
        a->right = node;
        a->refresh();
        node->parent = a;
        node->greaterThanParent = true;
        return a;
    }
}

// Devolve a raiz da BST
DynamicForest::Node* DynamicForest::Node::get_root() {
    DynamicForest::Node* node = this;
    while (node->parent != NULL) {
        node = node->parent;
    }
    return node;
}

// Faz com que o vertice u seja raiz de sua componente, isto e, que
// uu seja o primeiro elemento de sua sequencia eureliana
DynamicForest::Node* DynamicForest::reroot(int u) {
    DynamicForest::Node *uu = this->dict[{u, u}].get();
    size_t k = uu->order();
    DynamicForest::Node *root = uu->get_root();
    auto spl = split(root, k - 1);
    return merge(spl.second, spl.first);
}

} // namespace usp
