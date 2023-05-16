#ifndef LCA_HPP
#define LCA_HPP

#include <vector>

struct edge_t {
    int u, v;
};

using graph_t = std::vector<std::vector<edge_t> >;

class lca_t {
    static const int MAX_DEPTH = 20;

    size_t n;
    std::vector<std::vector<int> > up;
    std::vector<int> d;

    void dfs(const graph_t& g, int u, int prev) {
        for (const edge_t& elem : g[u]) {
            int v = elem.v;
            if (v != prev) {
                d[v] = d[u] + 1;
                up[v][0] = u;
                dfs(g, v, u);
            }
        }
    }

   public:
    lca_t(const graph_t& g, int root) : n(g.size()) {
        up.resize(n, std::vector<int>(MAX_DEPTH));
        d.resize(n);
        d[root] = 0;
        up[root][0] = root;
        dfs(g, root, -1);
        for (int j = 1; j < MAX_DEPTH; ++j) {
            for (size_t u = 0; u < n; ++u) {
                up[u][j] = up[up[u][j - 1]][j - 1];
            }
        }
    }

    lca_t() : n(1) {
        up.resize(n, std::vector<int>(MAX_DEPTH));
        d.resize(n);
        d[0] = 0;
    }

    void add_leaf(int prev, int u) {
        ++n;
        d.push_back(d[prev] + 1);
        up.push_back(std::vector<int>(MAX_DEPTH));
        up[u][0] = prev;
        for (int j = 1; j < MAX_DEPTH; ++j) {
            up[u][j] = up[up[u][j - 1]][j - 1];
        }
    }

    /* lca(x, y) */
    int find(int x, int y) {
        if (d[x] > d[y]) {
            std::swap(x, y);
        }
        int delta = d[y] - d[x];
        for (int j = MAX_DEPTH - 1; j >= 0; --j) {
            if (delta & (1 << j)) {
                y = up[y][j];
            }
        }
        if (x == y) {
            return x;
        }
        for (int j = MAX_DEPTH - 1; j >= 0; --j) {
            if (up[x][j] != up[y][j]) {
                x = up[x][j];
                y = up[y][j];
            }
        }
        return up[x][0];
    }

    /* path length from x to y */
    int dist(int x, int y) {
        if (d[x] > d[y]) {
            std::swap(x, y);
        }
        int delta = d[y] - d[x];
        for (int j = MAX_DEPTH - 1; j >= 0; --j) {
            if (delta & (1 << j)) {
                y = up[y][j];
            }
        }
        int res = delta;
        if (x == y) {
            return res;
        }
        for (int j = MAX_DEPTH - 1; j >= 0; --j) {
            if (up[x][j] != up[y][j]) {
                res = res + 2 * (1 << j);
                x = up[x][j];
                y = up[y][j];
            }
        }
        return res + 2;
    }
};

#endif /* LCA_HPP */
