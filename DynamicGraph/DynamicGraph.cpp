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
    edges[NON_TREE][u][l].erase(iterator[uv]);
    edges[NON_TREE][v][l].erase(iterator[vu]);
    iterator[uv] = edges[NON_TREE][u][l-1].insert(edges[NON_TREE][u][l-1].end(), v);
    iterator[vu] = edges[NON_TREE][v][l-1].insert(edges[NON_TREE][v][l-1].end(), u);
    level[uv] = level[vu] = l-1;
    F[l-1].mark_black(u);
    F[l-1].mark_black(v);
}

void DynamicGraph::promote(int u, int v, int l) {
    remove(u, v, NON_TREE);
    insert(u, v, l, TREE);
}

DynamicGraph::DynamicGraph(int n) :
        LOGN(ceil(log2(n))),
        F(vector<DynamicForest>(LOGN + 1, DynamicForest(n))) {
    edges[TREE] = vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1));
    edges[NON_TREE] = vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1));
}

void DynamicGraph::insert(int u, int v) {
    if (!F[LOGN].is_connected(u, v)) {
        insert(u, v, LOGN, TREE);
    } else {
        insert(u, v, LOGN, NON_TREE);
    }
}

void DynamicGraph::insert(int u, int v, int l, edge_type type) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    iterator[uv] = edges[type][u][l].insert(edges[type][u][l].end(), v);
    iterator[vu] = edges[type][v][l].insert(edges[type][v][l].end(), u);
    level[uv] = level[vu] = l;
    if (type == TREE) {
        F[l].mark_white(u);
        F[l].mark_white(v);
        for (int i = l; i <= LOGN; i++) {
            F[i].link(u, v);
        }
    } else {
        F[l].mark_black(u);
        F[l].mark_black(v);
    }
}

void DynamicGraph::remove(int u, int v) {
    int l = level[{u, v}];
    if (!F[LOGN].has_edge(u, v)) {
        remove(u, v, NON_TREE);
        return;
    }
    remove(u, v, TREE);
    for (int i = l; i <= LOGN; i++) {
        if (F[i].size(u) > F[i].size(v)) {
            std::swap(u, v);
        }
        downgrade_tree_edges(u, i);
        for (int x : F[i].black_nodes(u)) { // x eh um vertice de T_u em F[i]
            auto it = edges[NON_TREE][x][i].begin();
            while (it != edges[NON_TREE][x][i].end()) {
                int y = *it; // x-y eh uma aresta de nivel i, x pertence a T_u
                if (F[i].is_connected(x, y)) {
                    ++it; // importante fazer isso antes pq vou remover
                    downgrade_non_tree(x, y, i);
                } else {
                    promote(x, y, i);
                    return;
                }
            }
        }
    }
}

// Rebaixa todos as arestas de arvore de T_u em F_l
void DynamicGraph::downgrade_tree_edges(int u, int l) {
    for (int x : F[l].white_nodes(u)) {
        auto it = edges[TREE][x][l].begin();
        while (it != edges[TREE][x][l].end()) {
            int y = *it;
            ++it;
            pair<int, int> xy = {x, y};
            pair<int, int> yx = {y, x};
            F[l].unmark_white(x);
            F[l].unmark_white(y);
            edges[TREE][x][l].erase(iterator[xy]);
            edges[TREE][y][l].erase(iterator[yx]);
            F[l-1].link(x, y);
            F[l-1].mark_white(x);
            F[l-1].mark_white(y);
            iterator[xy] = edges[TREE][x][l-1].insert(edges[TREE][x][l-1].end(), y);
            iterator[yx] = edges[TREE][y][l-1].insert(edges[TREE][y][l-1].end(), x);
            level[xy] = level[yx] = l-1;
        }
    }
}

void DynamicGraph::remove(int u, int v, edge_type type) {
    int l = level[{u, v}];
    edges[type][u][l].erase(iterator[{u, v}]);
    edges[type][v][l].erase(iterator[{v, u}]);
    if (type == TREE) {
        F[l].unmark_white(u);
        F[l].unmark_white(v);
        for (int i = l; i <= LOGN; i++) {
            F[i].cut(u, v);
        }
    } else {
        F[l].unmark_black(u);
        F[l].unmark_black(v);
    }
}

bool DynamicGraph::is_connected(int u, int v) {
    return F[LOGN].is_connected(u, v);
}

} // namespace usp
