#ifndef TWO_SAT
#define TWO_SAT

#include <algorithm>
#include <vector>

struct two_sat_t {
    using vi = std::vector<int>;
    using graph = std::vector<vi>;

    int n;
    graph g, gt;
    two_sat_t(int n) : n(n), g(2 * n), gt(2 * n) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        gt[v].push_back(u);
    }

    void add_or(int x, int y) {
        int u = x < 0 ? 2 * ~x + 1 : 2 * x, v = y < 0 ? 2 * ~y + 1 : 2 * y;
        add_edge(u ^ 1, v);
        add_edge(v ^ 1, u);
    }

    int add_var() {
        for (int i = 0; i < 2; ++i) {
            g.emplace_back();
            gt.emplace_back();
        }
        return n++;
    }

    void at_most_one(const std::vector<int>& vars) {
        if (vars.size() < 2) {
            return;
        }
        int m = vars.size(), prev = vars[0];
        for (int i = 1; i < m - 1; ++i) {
            int next = add_var(), x = vars[i];
            add_or(~prev, ~x);
            add_or(~prev, next);
            add_or(~x, next);
            prev = next;
        }
        add_or(~prev, ~vars.back());
    }

    void dfs(int u) {
        if (vis[u]) {
            return;
        }
        vis[u] = true;
        for (int v : g[u]) {
            dfs(v);
        }
        order.push_back(u);
    }

    void scc(int u) {
        if (comps[u] != -1) {
            return;
        }
        comps[u] = id;
        for (int v : gt[u]) {
            scc(v);
        }
    }

    int id;
    vi order, vis, comps, res;
    bool solve() {
        order.clear();
        vis.assign(2 * n, 0);
        for (int i = 0; i < 2 * n; ++i) {
            dfs(i);
        }
        std::reverse(order.begin(), order.end());
        id = 0;
        comps.assign(2 * n, -1);
        for (int el : order) {
            scc(el);
            ++id;
        }
        res.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            if (comps[2 * i] == comps[2 * i + 1]) {
                return false;
            }
            res[i] = comps[2 * i] > comps[2 * i + 1];
        }
        return true;
    }
};

#endif /* TWO_SAT */
