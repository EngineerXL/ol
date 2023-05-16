#ifndef MAX_FLOW_HPP
#define MAX_FLOW_HPP

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

class flow_network_t {
   public:
    struct flow_edge_t {
        int u, v;
        int64_t c, f;

        flow_edge_t(int _u, int _v, int64_t _c) : u(_u), v(_v), c(_c), f() {}
    };

    const int64_t INF = 1e18;
    const int64_t MAX_B = 1 << 30;

    using vec = std::vector<int>;
    using vecsz = std::vector<size_t>;
    using graph = std::vector<vec>;
    using edges = std::vector<flow_edge_t>;

    int n;
    graph g;
    edges e;

    flow_network_t(int _n) : n(_n), g(n), d(n), ptr(n) {}

    void add_edge(int u, int v, int64_t cap) {
        flow_edge_t uv(u, v, cap);
        flow_edge_t vu(v, u, 0);
        g[u].push_back(e.size());
        e.push_back(uv);
        g[v].push_back(e.size());
        e.push_back(vu);
    }

    int64_t max_flow(int start, int finish) {
        clear_flow();
        int64_t res = 0;
        for (int64_t b = MAX_B; b > 0; b = b >> 1) {
            res += dinic(start, finish, b);
        }
        return res;
    }

   private:
    vec d;
    vecsz ptr;

    void build_network(int start, int finish, int64_t b) {
        d.assign(n, -1);
        d[start] = 0;
        std::queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == finish) {
                break;
            }
            for (int ind : g[u]) {
                int64_t c = e[ind].c;
                int64_t f = e[ind].f;
                int v = e[ind].v;
                if (c - f >= b and d[v] == -1) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    int64_t dfs_push(int u, int finish, int64_t b, int64_t flow) {
        if (u == finish) {
            return flow;
        }
        for (; ptr[u] < g[u].size(); ++ptr[u]) {
            int ind = g[u][ptr[u]];
            flow_edge_t uv = e[ind];
            int v = uv.v;
            if (d[u] + 1 != d[v]) {
                continue;
            }
            int64_t f = uv.f;
            int64_t c = uv.c;
            if (c - f >= b) {
                int64_t pushed = dfs_push(v, finish, b, std::min(flow, c - f));
                if (pushed) {
                    e[ind].f += pushed;
                    e[ind ^ 1].f -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    int64_t dinic(int start, int finish, int64_t b) {
        int64_t res = 0;
        while (1) {
            build_network(start, finish, b);
            if (d[finish] == -1) {
                break;
            }
            ptr.assign(n, 0);
            while (1) {
                int64_t flow = dfs_push(start, finish, b, INF);
                if (flow) {
                    res += flow;
                } else {
                    break;
                }
            }
        }
        return res;
    }

    void clear_flow() {
        for (flow_edge_t& elem : e) {
            elem.f = 0;
        }
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const flow_network_t& fn) {
        size_t m = fn.e.size();
        for (size_t i = 0; i < m; i += 2) {
            flow_edge_t elem = fn.e[i];
            int u = elem.u + 1;
            int v = elem.v + 1;
            int64_t f = elem.f;
            int64_t c = elem.c;
            out << u << ' ' << v << ' ' << f << '/' << c << '\n';
        }
        return out;
    }
};

#endif /* MAX_FLOW_HPP */