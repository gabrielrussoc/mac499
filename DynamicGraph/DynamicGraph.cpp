#include "DynamicGraph/DynamicGraph.hpp"

#include <cmath>

using std::list;
using std::pair;
using std::vector;

namespace usp {


void DynamicGraph::downgrade_non_tree(int u, int v, int l) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    F[l].unmark_black(u);
    F[l].unmark_black(v);
    non_tree_edges[u][l].erase(iterator[uv]);
    non_tree_edges[v][l].erase(iterator[vu]);
    iterator[uv] = non_tree_edges[u][l-1].insert(non_tree_edges[u][l-1].end(), v);
    iterator[vu] = non_tree_edges[v][l-1].insert(non_tree_edges[v][l-1].end(), u);
    level[uv] = level[vu] = l-1;
    F[l-1].mark_black(u);
    F[l-1].mark_black(v);
}

void DynamicGraph::replace(int u, int v, int l) {
    // Tem que fazer mais coisas.
    // uv agora eh uma aresta de arvore, precisa entrar nas paradas tudo
    remove_non_tree(u, v);
    insert_tree(u, v, l);
    for (int i = l; i <= LOGN; i++) {
        F[i].link(u, v);
    }
}

DynamicGraph::~DynamicGraph() {}

DynamicGraph::DynamicGraph(int n) :
        LOGN(ceil(log2(n))),
        tree_edges(vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1))),
        non_tree_edges(vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1))) {
    F = vector<DynamicForest>();
    F.reserve(LOGN+1);
    // TODO(gabrielrc): Arrumar o fato de que a DynamicForest nao pode ser copiada.
    for (int i = 0; i <= LOGN; i++) {
        F.emplace_back(n);
    }
}

void DynamicGraph::insert(int u, int v) {
    if (!F[LOGN].is_connected(u, v)) {
        F[LOGN].link(u, v);
        insert_tree(u, v, LOGN);
    } else {
        insert_non_tree(u, v);
    }
}

void DynamicGraph::insert_tree(int u, int v, int l) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    iterator[uv] = tree_edges[u][l].insert(tree_edges[u][l].end(), v);
    iterator[vu] = tree_edges[v][l].insert(tree_edges[v][l].end(), u);
    level[uv] = level[vu] = l;
    F[LOGN].mark_white(u);
    F[LOGN].mark_white(v);
}

// TODO(gabrielrc): Esse código é muito parecido com o de insert_tree. Reuse!
void DynamicGraph::insert_non_tree(int u, int v) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    iterator[uv] = non_tree_edges[u][LOGN].insert(non_tree_edges[u][LOGN].end(), v);
    iterator[vu] = non_tree_edges[v][LOGN].insert(non_tree_edges[v][LOGN].end(), u);
    level[uv] = level[vu] = LOGN;
    F[LOGN].mark_black(u);
    F[LOGN].mark_black(v);
}

void DynamicGraph::remove(int u, int v) {
    int l = level[{u, v}];
    if (!F[LOGN].has_edge(u, v)) {
        remove_non_tree(u, v);
        return;
    }
    remove_tree(u, v);
    for (int i = l; i <= LOGN; i++) {
        if (F[i].size(u) > F[i].size(v)) {
            std::swap(u, v);
        }
        downgrade_tree_edges(u, i);
        for (int x : F[i].black_nodes(u)) { // x eh um vertice de T_u em F[i]
            auto it = non_tree_edges[x][i].begin();
            while (it != non_tree_edges[x][i].end()) {
                int y = *it; // x-y eh uma aresta de nivel i, x pertence a T_u
                if (F[i].is_connected(x, y)) {
                    ++it; // importante fazer isso antes pq vou remover
                    downgrade_non_tree(x, y, i);
                } else {
                    replace(x, y, i);
                    return;
                }
            }
        }
    }
}

// Rebaixa todos as arestas de arvore de T_u em F_l
void DynamicGraph::downgrade_tree_edges(int u, int l) {
    for (int x : F[l].white_nodes(u)) {
        auto it = tree_edges[x][l].begin();
        while (it != tree_edges[x][l].end()) {
            int y = *it;
            ++it;
            pair<int, int> xy = {x, y};
            pair<int, int> yx = {y, x};
            F[l].unmark_white(x);
            F[l].unmark_white(y);
            tree_edges[x][l].erase(iterator[xy]);
            tree_edges[y][l].erase(iterator[yx]);
            F[l-1].link(x, y);
            F[l-1].mark_white(x);
            F[l-1].mark_white(y);
            iterator[xy] = tree_edges[x][l-1].insert(tree_edges[x][l-1].end(), y);
            iterator[yx] = tree_edges[y][l-1].insert(tree_edges[y][l-1].end(), x);
            level[xy] = level[yx] = l-1;
        }
    }
}

void DynamicGraph::remove_tree(int u, int v) {
    int l = level[{u, v}];
    tree_edges[u][l].erase(iterator[{u, v}]);
    tree_edges[v][l].erase(iterator[{v, u}]);
    // TODO(gabrielrc): Abstrair black/white para tree/non_tree
    F[l].unmark_white(u);
    F[l].unmark_white(v);
    for (int i = l; i <= LOGN; i++) {
        F[i].cut(u, v);
    }
}

void DynamicGraph::remove_non_tree(int u, int v) {
    int l = level[{u, v}];
    non_tree_edges[u][l].erase(iterator[{u, v}]);
    non_tree_edges[v][l].erase(iterator[{v, u}]);
    // TODO(gabrielrc): Abstrair black/white para tree/non_tree
    F[l].unmark_black(u);
    F[l].unmark_black(v);
}

bool DynamicGraph::is_connected(int u, int v) {
    return F[LOGN].is_connected(u, v);
}

} // namespace usp
