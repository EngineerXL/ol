#include <bits/stdc++.h>

#ifndef TREAP_H
#define TREAP_H

#include <ctime>
#include <iostream>

class treap_t {
    static const int64_t INF = 1e18;
    static const int TAB_SIZE = 4;

    struct treap_node_t {
        treap_node_t *_left;
        treap_node_t *_right;
        int _key;
        int64_t _priority;

        int64_t _value;
        int64_t _seg;

        treap_node_t(int key, int64_t value) {
            _left = nullptr;
            _right = nullptr;
            _key = key;
            _priority = std::rand();
            _value = value;
            _seg = value;
        }
    };

    using treap_ptr = treap_node_t *;

    treap_ptr root;

    int get_key(treap_ptr t) {
        if (t != nullptr) {
            return t->_key;
        } else {
            return 0;
        }
    }

    int64_t get_value(treap_ptr t) {
        if (t != nullptr) {
            return t->_value;
        } else {
            return -INF;
        }
    }

    int64_t get_seg(treap_ptr t) {
        if (t != nullptr) {
            return t->_seg;
        } else {
            return -INF;
        }
    }

    void update(treap_ptr t) {
        if (t != nullptr) {
            t->_key = 1 + get_key(t->_left) + get_key(t->_right);
            t->_seg = std::max(get_seg(t->_left), get_seg(t->_right));
            t->_seg = std::max(t->_seg, t->_value);
        }
    }

    void split(treap_ptr t, int key, treap_ptr &l, treap_ptr &r) {
        if (t == nullptr) {
            l = nullptr;
            r = nullptr;
            return;
        }
        int key_left_subtree = get_key(t->_left);
        if (key > key_left_subtree + 1) {
            split(t->_right, key - key_left_subtree - 1, t->_right, r);
            l = t;
        } else {
            split(t->_left, key, l, t->_left);
            r = t;
        }
        update(t);
    }

    treap_ptr merge(treap_ptr l, treap_ptr r) {
        if (l == nullptr) {
            return r;
        }
        if (r == nullptr) {
            return l;
        }
        if (l->_priority > r->_priority) {
            l->_right = merge(l->_right, r);
            update(l);
            return l;
        } else {
            r->_left = merge(l, r->_left);
            update(r);
            return r;
        }
    }

    void delete_treap(treap_ptr t) {
        if (t != nullptr) {
            delete_treap(t->_left);
            delete_treap(t->_right);
            delete t;
        }
    }

    friend std::ostream &operator<<(std::ostream &out, treap_t t) {
        print_treap(out, t.root, 0);
        return out;
    }

    friend std::ostream &operator<<(std::ostream &out, treap_ptr t) {
        print_treap(out, t, 0);
        return out;
    }

    friend void print_treap(std::ostream &out, treap_ptr t, int h) {
        if (t != nullptr) {
            print_treap(out, t->_left, h + 1);
            for (int i = 0; i < TAB_SIZE * h; ++i) {
                out << " ";
            }
            out << "{ x = " << t->_key << ", val = " << t->_value
                << ", seg = " << t->_seg << "}\n";
            print_treap(out, t->_right, h + 1);
        }
    }

   public:
    treap_t() {
        std::srand(std::time(NULL));
        root = nullptr;
    }

    void set(int key, int64_t value) {
        // std::cout << "set " << key << " " << value << std::endl;
        // std::cout << root << std::endl;
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key + 1, m, r);
        split(m, key, l, m);
        // std::cout << l << std::endl;
        // std::cout << m << std::endl;
        // std::cout << r << std::endl;
        m->_value = value;
        m->_seg = value;
        root = merge(merge(l, m), r);
    }

    int64_t get(int key_l, int key_r) {
        // std::cout << "get " << key_l << " " << key_r << std::endl;
        // std::cout << root << std::endl;
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key_r + 1, m, r);
        split(m, key_l, l, m);
        // std::cout << l << std::endl;
        // std::cout << m << std::endl;
        // std::cout << r << std::endl;
        int64_t ans = get_seg(m);
        root = merge(merge(l, m), r);
        return ans;
    }

    void insert(int key, int64_t value) {
        treap_ptr l = nullptr;
        treap_ptr r = nullptr;
        split(root, key + 1, l, r);
        treap_ptr m = new treap_node_t(1, value);
        root = merge(merge(l, m), r);
    }

    void erase(int key) {
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key + 1, m, r);
        split(m, key, l, m);
        root = merge(l, r);
        delete_treap(m);
    }

    void push_back(int64_t value) {
        treap_ptr m = new treap_node_t(1, value);
        root = merge(root, m);
    }

    void push_frond(int64_t value) {
        treap_ptr m = new treap_node_t(1, value);
        root = merge(m, root);
    }

    void destruct() { delete_treap(root); }
};

#endif /* TREAP_H */

#ifndef GRAPH_HPP
#define GRAPH_HPP

struct edge_t {
    int u, v;
};

struct wedge_t {
    int u, v;
    int64_t w;
};

using graph_t = std::vector<std::vector<int> >;
using wgraph_t = std::vector<std::vector<wedge_t> >;

void dfs_subtree_count(const graph_t &g, int u, int prev,
                       std::vector<int> &subtree_count) {
    int res = 0;
    for (size_t i = 0; i < g[u].size(); ++i) {
        int v = g[u][i];
        if (v != prev) {
            dfs_subtree_count(g, v, u, subtree_count);
            res = res + subtree_count[v];
        }
    }
    subtree_count[u] = 1 + res;
}

#ifndef LCA_HPP
#define LCA_HPP

#include <vector>

using graph_t = std::vector<std::vector<int> >;

class lca_t {
    static const int MAX_DEPTH = 20;

    size_t n;
    int timer;
    std::vector<int> tin;
    std::vector<int> tout;
    std::vector<std::vector<int> > up;

    void dfs_timer(const graph_t &g, int u, int prev) {
        tin[u] = ++timer;
        for (size_t i = 0; i < g[u].size(); ++i) {
            int v = g[u][i];
            if (v != prev) {
                dfs_timer(g, v, u);
                up[v][0] = u;
            }
        }
        tout[u] = ++timer;
    }

   public:
    lca_t(const graph_t &g) : n(g.size()), timer() {
        tin.resize(n);
        tout.resize(n);
        up.resize(n, std::vector<int>(MAX_DEPTH));
        dfs_timer(g, 0, -1);
        for (int j = 1; j < MAX_DEPTH; ++j) {
            for (size_t i = 0; i < n; ++i) {
                up[i][j] = up[up[i][j - 1]][j - 1];
            }
        }
    }

    int go_up(int u) { return up[u][0]; }

    bool is_upper(int x, int y) {
        return tin[x] <= tin[y] and tout[y] <= tout[x];
    }

    int find(int x, int y) {
        if (is_upper(x, y)) {
            return x;
        }
        if (is_upper(y, x)) {
            return y;
        }
        for (int j = MAX_DEPTH - 1; j >= 0; --j) {
            if (!is_upper(up[x][j], y)) {
                x = up[x][j];
            }
        }
        return up[x][0];
    }
};

#endif /* LCA_HPP */

class hld_t {
    static const int64_t INF = 1e18;
    static const int64_t LIGHT = 1;
    static const int64_t HEAVY = 2;

    size_t n;
    size_t n_path;
    std::vector<int> subtree_count;
    wgraph_t hlg;
    std::vector<std::vector<int> > path;
    std::vector<int> path_ind;
    std::vector<treap_t> path_data;
    std::vector<int> path_data_ind;

    void dfs_hlg(const graph_t &g, int u, int prev,
                 std::vector<wedge_t> &wedges) {
        for (size_t i = 0; i < g[u].size(); ++i) {
            int v = g[u][i];
            if (v != prev) {
                dfs_hlg(g, v, u, wedges);
                wedge_t cur_wedge = {u, v, LIGHT};
                if (subtree_count[v] * 2 >= subtree_count[u]) {
                    cur_wedge.w = HEAVY;
                }
                wedges.push_back(cur_wedge);
            }
        }
    }

    void build_hlg(const graph_t &g) {
        dfs_subtree_count(g, 0, -1, subtree_count);
        std::vector<wedge_t> wedges;
        dfs_hlg(g, 0, -1, wedges);
        for (size_t i = 0; i < wedges.size(); ++i) {
            int u = wedges[i].u;
            int v = wedges[i].v;
            int64_t w = wedges[i].w;
            hlg[u].push_back({u, v, w});
            hlg[v].push_back({v, u, w});
        }
    }

    void dfs_hld(int u, int prev) {
        for (size_t i = 0; i < hlg[u].size(); ++i) {
            int v = hlg[u][i].v;
            int64_t w = hlg[u][i].w;
            if (v != prev) {
                dfs_hld(v, u);
                if (w == HEAVY) {
                    path[path_ind[v]].push_back(u);
                    path_ind[u] = path_ind[v];
                }
            }
        }
        if (path_ind[u] == -1) {
            path.push_back({u});
            path_ind[u] = path.size() - 1;
        }
    }

   public:
    lca_t hlg_lca;

    hld_t(const graph_t &g, const std::vector<int64_t> &vertex_data)
        : hlg_lca(g) {
        n = g.size();
        hlg.resize(n);
        subtree_count.resize(n);
        path_ind.resize(n, -1);
        path_data.resize(n);
        path_data_ind.resize(n);
        build_hlg(g);
        dfs_hld(0, -1);
        n_path = path.size();
        for (size_t i = 0; i < n_path; ++i) {
            for (size_t j = 0; j < path[i].size(); ++j) {
                int u = path[i][j];
                path_data[i].push_back(vertex_data[u]);
                path_data_ind[u] = j + 1;
            }
        }
    }

    ~hld_t() {
        for (size_t i = 0; i < n_path; ++i) {
            path_data[i].destruct();
        }
    }

    void set(int u, int64_t value) {
        path_data[path_ind[u]].set(path_data_ind[u], value);
    }

    int64_t get(int x, int y) {
        if (hlg_lca.is_upper(x, y)) {
            int64_t res = -INF;
            int y_ind = path_ind[y];
            while (path_ind[x] != y_ind) {
                res = std::max(res, path_data[y_ind].get(path_data_ind[y],
                                                         path[y_ind].size()));
                y = hlg_lca.go_up(path[y_ind].back());
                y_ind = path_ind[y];
            }
            return std::max(
                res, path_data[y_ind].get(path_data_ind[y], path_data_ind[x]));
        } else {
            int lca_x_y = hlg_lca.find(x, y);
            return std::max(get(lca_x_y, x), get(lca_x_y, y));
        }
    }

    friend std::ostream &operator<<(std::ostream &out, const hld_t &hld) {
        out << "Heavy-light decomposition:\n";
        for (size_t i = 0; i < hld.n_path; ++i) {
            out << "Path " << i + 1 << ": ";
            for (size_t j = 0; j < hld.path[i].size() - 1; ++j) {
                out << hld.path[i][j] + 1 << ", ";
            }
            out << hld.path[i].back() + 1 << "\n";
        }
        return out;
    }
};

#endif /* GRAPH_HPP */

int main() {
    std::ios::sync_with_stdio(false);
    std::cout.tie(0);
    std::cin.tie(0);

    int n;
    std::cin >> n;
    int m = n - 1;
    graph_t g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u;
        --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    std::vector<int64_t> vertex_data(n);
    hld_t hld(g, vertex_data);
    // std::cout << hld << std::endl;
    int q;
    std::cin >> q;
    while (q--) {
        std::string s;
        int x, y;
        std::cin >> s >> x >> y;
        if (s == "I") {
            --x;
            hld.set(x, hld.get(x, x) + y);
        }
        if (s == "G") {
            --x;
            --y;
            std::cout << hld.get(x, y) << "\n";
        }
    }
}
