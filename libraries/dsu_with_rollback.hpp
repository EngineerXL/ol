#ifndef DSU_WITH_ROLLBACK_HPP
#define DSU_WITH_ROLLBACK_HPP

#include <array>
#include <cassert>
#include <numeric>
#include <stack>
#include <vector>

struct dsu_with_rollback_t {
    using pii = std::array<int, 2>;

    int n;
    std::vector<int> leader, size;
    std::stack<pii> ch_lead, ch_size;

   public:
    dsu_with_rollback_t(int _n) : n(_n), leader(n), size(n, 1) { std::iota(leader.begin(), leader.end(), 0); }

    int find(int u) {
        assert(0 <= u && u < n);
        return leader[u] == u ? u : find(leader[u]);
    }

    int get_size(int u) { return size[find(u)]; }

    void rollback() {
        assert(ch_size.size());
        auto [u, szu] = ch_size.top();
        ch_size.pop();
        size[u] = szu;
        auto [v, lev] = ch_lead.top();
        ch_lead.pop();
        leader[v] = lev;
    }

    bool join(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) {
            return false;
        }
        if (size[u] < size[v]) {
            std::swap(u, v);
        }
        ch_lead.push({v, leader[v]});
        ch_size.push({u, size[u]});
        leader[v] = u;
        size[u] = size[u] + size[v];
        return true;
    }
};

using dsu_rb = dsu_with_rollback_t;

#endif /* DSU_WITH_ROLLBACK_HPP */
