#include "DynamicGraph/DynamicGraph.hpp"

#include <cmath>

using std::list;
using std::pair;
using std::vector;

namespace usp {

DynamicGraph::DynamicGraph(int n) :
        LOGN(ceil(log2(n))),
        F(vector<DynamicForest>(LOGN + 1, DynamicForest(n))) {
    edges[TREE] = vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1));
    edges[NON_TREE] = vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1));
}

// Insere a aresta uv no grafo. Supoe que uv nao existe.
void DynamicGraph::insert(int u, int v) {
    if (!F[LOGN].is_connected(u, v)) {
        insert(u, v, LOGN, TREE);
    } else {
        insert(u, v, LOGN, NON_TREE);
    }
}

// Remove a aresta uv do grafo. Supoe que uv existe.
void DynamicGraph::remove(int u, int v) {
    if (!F[LOGN].has_edge(u, v)) {
        remove(u, v, NON_TREE);
        return;
    }
    remove(u, v, TREE);
    int l = level[{u, v}];
    for (int i = l; i <= LOGN; i++) {
        if (F[i].size(u) > F[i].size(v)) {
            std::swap(u, v);
        }
        downgrade_tree_edges(u, i);
        for (int x : F[i].black_nodes(u)) { // x eh um vertice de T_u em F[i]
            auto it = edges[NON_TREE][x][i].begin();
            while (it != edges[NON_TREE][x][i].end()) {
                int y = *it++; // x-y eh uma aresta de nivel i, x pertence a T_u
                if (F[i].is_connected(x, y)) {
                    downgrade(x, y, i, NON_TREE);
                } else {
                    remove(x, y, NON_TREE);
                    insert(x, y, i, TREE);
                    return;
                }
            }
        }
    }
}

// Devolve true se u e v estao conectados no grafo
bool DynamicGraph::is_connected(int u, int v) {
    return F[LOGN].is_connected(u, v);
}

// Insere a aresta uv do tipo type no nivel l
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

// Remove a aresta uv do tipo type
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

// Rebaixa o nível de todos as arestas de nivel l
// na componente de u em F_l
void DynamicGraph::downgrade_tree_edges(int u, int l) {
    for (int x : F[l].white_nodes(u)) {
        auto it = edges[TREE][x][l].begin();
        while (it != edges[TREE][x][l].end()) {
            int y = *it++;
            downgrade(x, y, l, TREE);
        }
    }
}

// Rebaixa o nivel da aresta uv do tipo type no nível l
void DynamicGraph::downgrade(int u, int v, int l, edge_type type) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    edges[type][u][l].erase(iterator[uv]);
    edges[type][v][l].erase(iterator[vu]);
    iterator[uv] = edges[type][u][l-1].insert(edges[type][u][l-1].end(), v);
    iterator[vu] = edges[type][v][l-1].insert(edges[type][v][l-1].end(), u);
    level[uv] = level[vu] = l-1;
    if (type == TREE) {
        F[l].unmark_white(u);
        F[l].unmark_white(v);
        F[l-1].link(u, v);
        F[l-1].mark_white(u);
        F[l-1].mark_white(v);
    } else {
        F[l].unmark_black(u);
        F[l].unmark_black(v);
        F[l-1].mark_black(u);
        F[l-1].mark_black(v);
    }
}

} // namespace usp
