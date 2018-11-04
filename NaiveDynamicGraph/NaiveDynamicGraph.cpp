#include "NaiveDynamicGraph/NaiveDynamicGraph.hpp"

#include <queue>

using std::pair;
using std::queue;
using std::vector;

namespace usp {

NaiveDynamicGraph::NaiveDynamicGraph(size_t n) :
    n(n),
    adj(vector<vector<bool>>(n, vector<bool>(n, false))) {}

void NaiveDynamicGraph::insert(int u, int v) {
    adj[u][v] = adj[v][u] = 1;
}

void NaiveDynamicGraph::remove(int u, int v) {
    adj[u][v] = adj[v][u] = 0;
}

bool NaiveDynamicGraph::is_connected(int u, int v) const {
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(u);
    vis[u] = true;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nxt = 0; nxt < n; nxt++) {
            if (adj[cur][nxt] && !vis[nxt]) {
                q.push(nxt);
                vis[nxt] = true;
            }
        }
    }
    return vis[v];
}

bool NaiveDynamicGraph::has_edge(int u, int v) const {
    return adj[u][v];
}

size_t NaiveDynamicGraph::size() const {
    return n;
}

vector<pair<int, int>> NaiveDynamicGraph::edges() const {
    vector<pair<int, int>> edges;
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (adj[u][v]) {
                edges.push_back({u, v});
            }
        }
    }
    return edges;
}

NaiveDynamicGraph NaiveDynamicGraph::grid(int n, int m) {
    NaiveDynamicGraph dg(n * m);
    int di[] = {1, -1, 0, 0};
    int dj[] = {0, 0, 1, -1};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int u = i * m + j;
            for (int k = 0; k < 4; k++) {
                int ni = i + di[k];
                int nj = j + dj[k];
                if (0 <= ni && ni < n && 0 <= nj && nj < m) {
                    int v = ni * m + nj;
                    dg.insert(u, v);
                }
            }
        }
    }
    return dg;
}

} // namespace usp
