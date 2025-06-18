#ifndef DSU_HPP
#define DSU_HPP

#include <cassert>
#include <numeric>
#include <vector>

struct dsu_t {
    int n;
    std::vector<int> p;
    std::vector<int> sz;

   public:
    dsu_t(int n) : n(n), p(n), sz(n, 1) { std::iota(p.begin(), p.end(), 0); }

    int find(int u) { return u == p[u] ? u : p[u] = find(p[u]); }

    int get_size(int u) { return sz[find(u)]; }

    bool join(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) std::swap(u, v);
        sz[u] += sz[v];
        p[v] = u;
        return true;
    }

    bool is_same(int u, int v) { return find(u) == find(v); }
};

#endif /* DSU_HPP */