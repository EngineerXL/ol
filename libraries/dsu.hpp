#ifndef DSU_HPP
#define DSU_HPP

#include <vector>

template <class T>
class dsu_t {
    std::vector<T> leader;
    std::vector<T> size;

   public:
    dsu_t(const size_t n) {
        leader.resize(n);
        size.resize(n);
        for (size_t i = 0; i < n; ++i) {
            leader[i] = i;
            size[i] = 1;
        }
    }

    T find(const T &u) {
        T u_leader = leader[u];
        if (u_leader != u) {
            leader[u] = find(u_leader);
        }
        return leader[u];
    }

    T get_size(const T &u) { return size[find(u)]; }

    bool join(const T &u, const T &v) {
        T u_leader = find(u);
        T v_leader = find(v);
        if (u_leader == v_leader) {
            return false;
        }
        if (size[u_leader] < size[v_leader]) {
            swap(u_leader, v_leader);
        }
        size[u_leader] = size[u_leader] + size[v_leader];
        leader[v_leader] = u_leader;
        return true;
    }
};

#endif /* DSU_HPP */