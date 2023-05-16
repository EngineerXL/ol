#ifndef LCA_FOREST_HPP
#define LCA_FOREST_HPP

#include <assert.h>

#include <vector>

template <class T>
struct wedge_t {
    int u, v;
    T w;

    wedge_t(int _u, int _v, const T& _w) : u(_u), v(_v), w(_w) {}
};

template <class T>
class graph_t {
   public:
    using edge = wedge_t<T>;

    int n;
    std::vector<edge> e;
    std::vector<std::vector<int> > g;

    graph_t(int _n) : n(_n), g(_n) {}

    virtual int add_edge(int u, int v, T w) = 0;
};

template <class T>
class forest_t : public graph_t<T> {
   public:
    using graph_t<T>::e;
    using graph_t<T>::g;
    using graph_t<T>::n;

    forest_t(int _n) : graph_t<T>(_n) {}

    int add_edge(int u, int v, T w = 1) {
        assert(0 <= u and u < n);
        assert(0 <= v and v < n);
        int id = e.size();
        assert(id < n - 1);
        g[u].push_back(id);
        g[v].push_back(id);
        e.push_back(wedge_t<T>(u, v, w));
        return id;
    }
};

template <class T>
class dfs_forest_t : public forest_t<T> {
   public:
    using forest_t<T>::e;
    using forest_t<T>::g;
    using forest_t<T>::n;

    std::vector<int> depth;
    std::vector<T> w_to_p;
    std::vector<int> parent;
    std::vector<int> sz;
    std::vector<int> tin, tout;
    int timer;

    dfs_forest_t(int _n)
        : forest_t<T>(_n),
          depth(n, -1),
          w_to_p(n),
          parent(n),
          sz(n, 1),
          tin(n),
          tout(n) {}

    void dfs_all() {
        for (int i = 0; i < n; ++i) {
            if (depth[i] != -1) {
                continue;
            }
            dfs_from(i);
        }
    }

    void dfs_from(int u) {
        depth[u] = u;
        parent[u] = u;
        timer = 0;
        dfs(u);
    }

    /* is x ancestor of y? */
    bool is_upper(int x, int y) {
        return tin[x] <= tin[y] and tout[y] <= tout[x];
    }

   private:
    void dfs(int u) {
        tin[u] = ++timer;
        for (int id : g[u]) {
            auto elem = e[id];
            int v = elem.u ^ elem.v ^ u;
            if (v == parent[u]) {
                continue;
            }
            w_to_p[v] = elem.w;
            depth[v] = 1 + depth[u];
            parent[v] = u;
            dfs(v);
            sz[u] += sz[v];
        }
        tout[u] = ++timer;
    }
};

template <class T>
class lca_forest_t : public dfs_forest_t<T> {
    using dfs_forest_t<T>::e;
    using dfs_forest_t<T>::g;
    using dfs_forest_t<T>::n;
    using dfs_forest_t<T>::depth;
    using dfs_forest_t<T>::w_to_p;
    using dfs_forest_t<T>::parent;
    using dfs_forest_t<T>::sz;
    using dfs_forest_t<T>::tin;
    using dfs_forest_t<T>::tout;

    using dfs_forest_t<T>::dfs_all;
    using dfs_forest_t<T>::dfs_from;
    using dfs_forest_t<T>::is_upper;

    int h;
    std::vector<std::vector<int> > up;
    std::vector<std::vector<T> > dist;

   public:
    lca_forest_t(int _n) : dfs_forest_t<T>(_n) {}

    void build_lca(int root = -1) {
        if (root == -1) {
            dfs_all();
        } else {
            dfs_from(root);
        }
        int max_depth = 0;
        for (int i = 0; i < n; ++i) {
            max_depth = std::max(max_depth, depth[i]);
        }
        h = 0;
        while ((1 << h) <= max_depth) {
            ++h;
        }
        up.resize(n, std::vector<int>(h));
        dist.resize(n, std::vector<T>(h));
        for (int i = 0; i < n; ++i) {
            up[i][0] = parent[i];
            dist[i][0] = w_to_p[i];
        }
        for (int j = 1; j < h; ++j) {
            for (int i = 0; i < n; ++i) {
                up[i][j] = up[up[i][j - 1]][j - 1];
                dist[i][j] = dist[i][j - 1] + dist[up[i][j - 1]][j - 1];
            }
        }
    }

    int go_up(int u, int dist = 1) {
        dist = std::min((1 << h) - 1, dist);
        for (int i = 0; i < h; ++i) {
            if (dist & (1 << i)) {
                u = up[u][i];
            }
        }
        return u;
    }

    T dist_up(int x, int y) {
        if (x == y) {
            return T();
        }
        T res;
        for (int j = h - 1; j >= 0; --j) {
            if (!is_upper(up[x][j], y)) {
                res = res + dist[x][j];
                x = up[x][j];
            }
        }
        return res + dist[x][0];
    }

    T dist_on_path(int x, int y) {
        int u = find(x, y);
        return dist_up(x, u) + dist_up(y, u);
    }

    int find(int x, int y) {
        if (is_upper(x, y)) {
            return x;
        }
        if (is_upper(y, x)) {
            return y;
        }
        for (int j = h - 1; j >= 0; --j) {
            if (!is_upper(up[x][j], y)) {
                x = up[x][j];
            }
        }
        return up[x][0];
    }
};

#endif /* LCA_FOREST_HPP */