#ifndef DSU_HPP
#define DSU_HPP

#include <cassert>
#include <vector>

struct dsu_t {
    int n;
    std::vector<int> leader;
    std::vector<int> size;
    std::vector<std::vector<int> > elems;

   public:
    dsu_t(const int _n)
        : n(_n), leader(n), size(n, 1), elems(n, std::vector<int>(1)) {
        for (int i = 0; i < n; ++i) {
            leader[i] = i;
            elems[i][0] = i;
        }
    }

    int find(const int &u) {
        assert(0 <= u and u < n);
        int u_leader = leader[u];
        if (u_leader != u) {
            leader[u] = find(u_leader);
        }
        return leader[u];
    }

    int get_size(const int &u) { return size[find(u)]; }

    std::vector<int> get_elems(const int &u) { return elems[find(u)]; }

    bool join(const int &u, const int &v) {
        int u_leader = find(u);
        int v_leader = find(v);
        if (u_leader == v_leader) {
            return false;
        }
        if (size[u_leader] < size[v_leader]) {
            std::swap(u_leader, v_leader);
        }
        size[u_leader] = size[u_leader] + size[v_leader];
        for (int el : elems[v_leader]) {
            elems[u_leader].emplace_back(el);
        }
        /* Don't forget to clean up memory */
        elems[v_leader].resize(0);
        leader[v_leader] = u_leader;
        return true;
    }
};

#endif /* DSU_HPP */