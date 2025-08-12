#ifndef LCA_DEPTH_HPP
#define LCA_DEPTH_HPP

#include <array>
#include <vector>

/*
 * Simple LCA struct, supports leaf addition
 * T is an edge type, modify dfs to match it
 * By default dfs works with struct (v_to, e_weight)
 */
template <class T>
struct lca_t {
    static const int L = 20;
    using ar = std::array<int, L>;
    using graph = std::vector<std::vector<T> >;

    int n;
    std::vector<ar> up;
    std::vector<int> d;

    void dfs(const graph& g, int u, int prev) {
        for (const auto& [v, _] : g[u]) {
            if (v == prev) continue;
            d[v] = d[u] + 1;
            up[v][0] = u;
            dfs(g, v, u);
        }
    }

    // Set root = -1 to handle forest
    lca_t(const graph& g, int root = 0) : lca_t(g.size(), root) {
        if (root != -1)
            dfs(g, root, root);
        else {
            for (int i = 0; i < n; ++i) {
                up[i][0] = -1;
            }
            for (int i = 0; i < n; ++i) {
                if (up[i][0] != -1) continue;
                up[i][0] = i;
                dfs(g, i, i);
            }
        }
        for (int j = 1; j < L; ++j) {
            for (int u = 0; u < n; ++u) {
                up[u][j] = up[up[u][j - 1]][j - 1];
            }
        }
    }

    lca_t(int n = 1, int root = 0) : n(n), up(n), d(n) {
        if (root != -1) up[root][0] = root;
    }

    void add_leaf(int prev, int u) {
        ++n;
        d.push_back(d[prev] + 1);
        up.emplace_back();
        up[u][0] = prev;
        for (int j = 1; j < L; ++j) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }
    }

    // Walk dist edges up from u
    int lift(int u, int dist) {
        for (int j = L - 1; j >= 0; --j) {
            if (dist & (1 << j)) u = up[u][j];
        }
        return u;
    }

    // Find lca(x, y)
    int find(int x, int y) {
        if (d[x] > d[y]) std::swap(x, y);
        y = lift(y, d[y] - d[x]);
        if (x == y) return x;
        for (int j = L - 1; j >= 0; --j) {
            if (up[x][j] != up[y][j]) {
                x = up[x][j];
                y = up[y][j];
            }
        }
        return up[x][0];
    }
};

#endif /* LCA_DEPTH_HPP */