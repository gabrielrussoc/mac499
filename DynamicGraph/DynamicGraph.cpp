#include "DynamicGraph/DynamicGraph.hpp"

#include <cmath>

using std::list;
using std::pair;
using std::vector;

namespace usp {

void DynamicGraph::downgrade(int u, int v, int l) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    F[l].unmark(u);
    F[l].unmark(v);
    adj[u][l].erase(iterator[uv]);
    adj[v][l].erase(iterator[vu]);
    l--;
    //acho que tem q rebaixar todas as arestas da árvore antes.
    if (!F[l].is_connected(u, v)) {
        F[l].link(u, v);
    }
    iterator[uv] = adj[u][l].insert(adj[u][l].end(), v);
    iterator[vu] = adj[v][l].insert(adj[v][l].end(), u);
    level[uv] = level[vu] = l;
    F[l].mark(u);
    F[l].mark(v);
}

void DynamicGraph::replace(int u, int v, int l) {
    for (int i = l; i <= LOGN; i++) {
        F[i].link(u, v);
    }
}

DynamicGraph::~DynamicGraph() {}

DynamicGraph::DynamicGraph(int n) : LOGN(ceil(log2(n))) {
    F = vector<DynamicForest>();
    F.reserve(LOGN+1);
    // TODO(gabrielrc): Arrumar o fato de que a DynamicForest nao pode ser copiada.
    for (int i = 0; i <= LOGN; i++) {
        F.emplace_back(n);
    }
    adj = vector<vector<list<int>>>(n, vector<list<int>>(LOGN + 1));
}

void DynamicGraph::insert(int u, int v) {
    pair<int, int> uv = {u, v};
    pair<int, int> vu = {v, u};
    iterator[uv] = adj[u][LOGN].insert(adj[u][LOGN].end(), v);
    iterator[vu] = adj[v][LOGN].insert(adj[v][LOGN].end(), u);
    level[uv] = level[vu] = LOGN;
    F[LOGN].mark(u);
    F[LOGN].mark(v);
    if (!F[LOGN].is_connected(u, v)) {
        F[LOGN].link(u, v);
    }
}

void DynamicGraph::remove(int u, int v) {
    int l = level[{u, v}];
    adj[u][l].erase(iterator[{u, v}]);
    adj[v][l].erase(iterator[{v, u}]);
    F[l].unmark(u);
    F[l].unmark(v);
    if (!F[LOGN].has_edge(u, v)) {
        return;
    }
    for (int i = l; i <= LOGN; i++) {
        F[i].cut(u, v);
    }
    for (int i = l; i <= LOGN; i++) {
        if (F[i].size(u) > F[i].size(v)) {
            std::swap(u, v);
        }
        for (int x : F[i].all_marked(u)) { // x eh um vertice de T_u em F[i]
            auto it = adj[x][i].begin();
            while (it != adj[x][i].end()) {
                int y = *it; // x-y eh uma aresta de nivel i, x pertence a T_u
                if (F[i].is_connected(x, y)) {
                    ++it; // importante fazer isso antes pq vou remover
                    downgrade(x, y, i);
                } else {
                    replace(x, y, i);
                    return;
                }
            }
        }
    }
}

bool DynamicGraph::is_connected(int u, int v) {
    return F[LOGN].is_connected(u, v);
}

} // namespace usp
