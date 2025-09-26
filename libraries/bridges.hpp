#ifndef BRIDGES_HPP
#define BRIDGES_HPP

#include <algorithm>
#include <vector>

// Finds bridges in undirected graph
// g should be adj list of (to, edge_id)
// f is a callback function to process edges
// Complexity: O(n + m)
template <class T, class Func>
void find_bridges(const std::vector<std::vector<T>>& g, Func f) {
    int n = g.size();
    std::vector<int> d(n, -1), low(n, -1);
    auto dfs = [&](auto&& dfs, int u, int par_eid) -> void {
        low[u] = d[u];
        for (const auto& [v, eid] : g[u]) {
            if (eid == par_eid) continue;
            if (d[v] == -1) {
                d[v] = d[u] + 1;
                dfs(dfs, v, eid);
                if (low[v] == d[u] + 1) f(eid);
            }
            low[u] = std::min(low[u], low[v]);
        }
    };
    for (int i = 0; i < n; ++i)
        if (d[i] == -1) {
            d[i] = 0;
            dfs(dfs, 0, -1);
        }
}

#endif /* BRIDGES_HPP */