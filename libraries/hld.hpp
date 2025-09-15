#ifndef HLD_HPP
#define HLD_HPP

#include <algorithm>
#include <cassert>
#include <vector>

// Heavy-light decomposition of a tree
// Builds tree decomposition such that any leaf contains log(n) paths to the root
// Supports path query and modification (vals at vertices and vals in the edges) O(log^2(n))
// Supports subtree query and modification (vals at vertices only) O(log(n))
template <bool VALS_ON_EDGES, class T>
struct hld_t {
    std::vector<std::vector<int>> g;
    int n, timer;
    lazy_segment_tree_t<T> tr;
    std::vector<int> sz, par, nxt, tin, tout;

    void dfs_sz(int u) {
        for (int& v : g[u]) {
            // Erase parent in child node
            g[v].erase(std::find(g[v].begin(), g[v].end(), u));
            dfs_sz(v);
            sz[u] += sz[v];
            if (sz[v] > sz[g[u][0]]) std::swap(g[u][0], v);
        }
    }

    void dfs(int u) {
        tin[u] = timer++;
        for (int v : g[u]) {
            nxt[v] = v != g[u][0] ? v : nxt[u];
            par[v] = u;
            dfs(v);
        }
        tout[u] = timer;
    }

    // Change if needed
    constexpr static T INIT_TR_VALS = 0;
    constexpr static T INIT_GET_VAL = 0;

    // Bidirectional adjancy list of a tree, root is 0
    hld_t(const std::vector<std::vector<int>>& g)
        : g(g),
          n(g.size()),
          timer(0),
          tr(n, INIT_TR_VALS),
          sz(n, 1),
          par(n),
          nxt(n),
          tin(n),
          tout(n) {
        dfs_sz(0);
        dfs(0);
        tr.nullval = INIT_GET_VAL;
    }

    // is x ancestor of y?
    bool is_upper(int x, int y) { return tin[x] <= tin[y] and tout[y] <= tout[x]; }

    template <class Func>
    void lift(int& u, int v, Func f) {
        while (!is_upper(nxt[u], v)) {
            f(tin[nxt[u]], tin[u] + 1);
            u = par[nxt[u]];
        }
    }

    template <class Func>
    void proc(int u, int v, Func f) {
        lift(u, v, f);
        lift(v, u, f);
        if (tin[u] > tin[v]) std::swap(u, v);
        f(tin[u] + VALS_ON_EDGES, tin[v] + 1);
    }

    // Update value on path
    void upd_path(int u, int v, const T& val) {
        proc(u, v, [&](int lo, int hi) { tr.upd(lo, hi, val); });
    }

    // Get value on path
    T get_path(int u, int v) {
        T res = INIT_GET_VAL;
        proc(u, v, [&](int lo, int hi) { res = tr.f(res, tr.get(lo, hi)); });
        return res;
    }

    // // Update value at vertex or on edge to parent
    void upd_vertex(int u, const T& val) { tr.upd(tin[u], tin[u] + 1, val); }

    // Update value in subtree
    void upd_sub(int u, const T& val) {
        assert(!VALS_ON_EDGES);
        tr.upd(tin[u], tout[u], val);
    }

    // Get value in subtree
    T get_sub(int u) {
        assert(!VALS_ON_EDGES);
        return tr.get(tin[u], tout[u]);
    }
};

#endif /* HLD_HPP */
