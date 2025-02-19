#ifndef MAX_FLOW_HPP
#define MAX_FLOW_HPP

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

// Flow network
struct flow_network_t {
    using ll = long long;
    using vi = std::vector<int>;

    // Directed edge u -> v with capacity c and flow value f
    struct edge_t {
        int u, v;
        ll c, f;

        edge_t(int u, int v, ll c) : u(u), v(v), c(c), f() {}
    };

    int n, source, sink;
    vi d, ptr;
    std::vector<vi> g;
    std::vector<edge_t> e;
    static const ll INF = 1e18;

    flow_network_t(int n) : n(n), d(n), ptr(n), g(n) {}

    // Adds directed edge u -> v with capacity c
    void add_edge(int u, int v, ll c) {
        g[u].push_back(e.size());
        e.emplace_back(u, v, c);
        g[v].push_back(e.size());
        e.emplace_back(v, u, 0);
    }

    // Finds the maximum flow from vertex s to vertex t
    ll max_flow(int s, int t) {
        source = s, sink = t;
        clear_flow();
        ll max_c = 0, res = 0;
        for (auto [u, v, c, f] : e) max_c = std::max(max_c, c);
        for (ll b = max_c; b > 0; b >>= 1) res += dinic(b);
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const flow_network_t& fn) {
        for (int i = 0; i < fn.n; ++i) {
            for (int j : fn.g[i]) {
                if (j & 1) continue;
                auto [u, v, c, f] = fn.e[j];
                out << u + 1 << ' ' << v + 1 << ' ' << f << '/' << c << '\n';
            }
        }
        return out;
    }

    // Performs BFS in residual graph
    void bfs(ll b) {
        d.assign(n, -1);
        d[source] = 0;
        std::queue<int> q;
        q.push(source);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == sink) break;
            for (int ind : g[u]) {
                ll c = e[ind].c;
                ll f = e[ind].f;
                int v = e[ind].v;
                if (c - f >= b and d[v] == -1) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    // Finds augment path form sink to source
    ll dfs_push(int u, ll b, ll flow) {
        if (u == sink) return flow;
        for (; ptr[u] < (int)g[u].size(); ++ptr[u]) {
            int ind = g[u][ptr[u]];
            auto [_, v, c, f] = e[ind];
            if (d[u] + 1 != d[v]) continue;
            if (c - f >= b) {
                ll pushed = dfs_push(v, b, std::min(flow, c - f));
                if (pushed) {
                    e[ind].f += pushed;
                    e[ind ^ 1].f -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }

    ll dinic(ll b) {
        ll res = 0;
        while (1) {
            bfs(b);
            if (d[sink] == -1) break;
            ptr.assign(n, 0);
            while (1) {
                ll flow = dfs_push(source, b, INF);
                if (flow)
                    res += flow;
                else
                    break;
            }
        }
        return res;
    }

    void clear_flow() {
        for (auto& [u, v, c, f] : e) f = 0;
    }
};

#endif /* MAX_FLOW_HPP */
