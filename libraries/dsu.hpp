#ifndef DSU_HPP
#define DSU_HPP

#include <cassert>
#include <numeric>
#include <vector>

struct dsu_t {
    int n;
    std::vector<int> leader;
    std::vector<int> size;

   public:
    dsu_t(const int _n) : n(_n), leader(n), size(n, 1) {
        std::iota(leader.begin(), leader.end(), 0);
    }

    int find(int u) {
        assert(0 <= u and u < n);
        int u_leader = leader[u];
        if (u_leader != u) {
            leader[u] = find(u_leader);
        }
        return leader[u];
    }

    int get_size(int u) { return size[find(u)]; }

    bool join(int u, int v) {
        int u_leader = find(u);
        int v_leader = find(v);
        if (u_leader == v_leader) {
            return false;
        }
        if (size[u_leader] < size[v_leader]) {
            std::swap(u_leader, v_leader);
        }
        size[u_leader] = size[u_leader] + size[v_leader];
        leader[v_leader] = u_leader;
        return true;
    }
};

#endif /* DSU_HPP */