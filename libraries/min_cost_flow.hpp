#ifndef MIN_COST_FLOW_HPP
#define MIN_COST_FLOW_HPP

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

// Minimum cost flow
// F - type of flow and capacity
// C - type of cost
template <class F, class C>
struct mcf_t {
    using vi = std::vector<int>;
    using item = std::pair<C, int>;

    // Directed edge u -> v with capacity c and flow value f and cost w
    struct edge_t {
        int u, v;
        F c, f;
        C w;

        edge_t(int u, int v, F c, C w) : u(u), v(v), c(c), f(0), w(w) {}
    };

    F flow;
    C cost;
    int n, source, sink;
    bool ford_done;
    std::vector<C> pot, npot, d;
    vi vis, prev;
    std::vector<vi> g;
    std::vector<edge_t> e;
    const C INF = std::numeric_limits<C>::max();
    std::priority_queue<item, std::vector<item>, std::greater<item> > pq;

    mcf_t(int n, int source, int sink)
        : flow(0),
          cost(0),
          n(n),
          source(source),
          sink(sink),
          ford_done(false),
          pot(n),
          npot(n),
          d(n),
          vis(n),
          g(n) {}

    // Adds directed edge u -> v with capacity c and cost w
    void add_edge(int u, int v, F c, C w) {
        g[u].push_back(e.size());
        e.emplace_back(u, v, c, w);
        g[v].push_back(e.size());
        e.emplace_back(v, u, 0, -w);
    }

    // Performs Ford-Bellman algo on residual network
    // returns true if sink is reachable from source
    bool ford_bellman() {
        pot.assign(n, INF);
        pot[source] = 0;
        bool changed = true;
        for (int i = 0; i < n && changed; ++i) {
            changed = false;
            for (auto [u, v, c, f, w] : e) {
                if (f < c && pot[u] < INF && pot[u] + w < pot[v]) {
                    pot[v] = pot[u] + w;
                    changed = true;
                }
            }
        }
        assert(!changed);
        ford_done = true;
        return pot[sink] < INF;
    }

    // Performs Dijkstra's algo on residual network
    // returns true if sink is reachable from source
    bool dijkstra(bool use_pq) {
        d.assign(n, INF);
        d[source] = 0;
        vis.assign(n, 0);
        prev.assign(n, -1);
        if (use_pq) {
            pq.push({0, source});
            while (pq.size()) {
                auto [_, u] = pq.top();
                pq.pop();
                if (vis[u]) continue;
                vis[u] = true;
                for (int id : g[u]) {
                    auto [_, v, c, f, w] = e[id];
                    w = w + pot[u] - pot[v];
                    if (f < c && d[u] + w < d[v]) {
                        d[v] = d[u] + w;
                        pq.push({d[v], v});
                        prev[v] = id;
                    }
                }
            }
        } else {
            for (int i = 0; i < n; ++i) {
                int u = -1;
                for (int j = 0; j < n; ++j)
                    if (!vis[j] && d[j] < INF && (u == -1 || d[j] < d[u]))
                        u = j;
                if (u == -1) break;
                vis[u] = true;
                for (int id : g[u]) {
                    auto [_, v, c, f, w] = e[id];
                    w = w + pot[u] - pot[v];
                    if (f < c && d[u] + w < d[v]) {
                        d[v] = d[u] + w;
                        prev[v] = id;
                    }
                }
            }
        }
        // Calculate new potentials
        for (int i = 0; i < n; i++) npot[i] = d[i] < INF ? d[i] + pot[i] : INF;
        pot = npot;
        return d[sink] < INF;
    }

    // Calculates minimum cost flow <= flow_limit
    std::pair<F, C> get(F flow_limit, bool use_pq = true) {
        if (!ford_done && !ford_bellman()) return {0, 0};
        while (flow < flow_limit) {
            if (!dijkstra(use_pq)) break;
            F df = flow_limit - flow;
            C dc = 0;
            int u = sink;
            while (u != source) {
                int id = prev[u];
                auto [v, _, c, f, w] = e[id];
                df = std::min(df, c - f);
                dc += w;
                u = v;
            }
            flow += df;
            cost += df * dc;
            u = sink;
            while (u != source) {
                int id = prev[u];
                e[id].f += df;
                e[id ^ 1].f -= df;
                u = e[id].u;
            }
        }
        return {flow, cost};
    }

    // Calculates minimum cost maximum flow
    std::pair<F, C> get_max_flow(bool use_pq = true) {
        return get(std::numeric_limits<F>::max(), use_pq);
    }
};

#endif /* MIN_COST_FLOW_HPP */
