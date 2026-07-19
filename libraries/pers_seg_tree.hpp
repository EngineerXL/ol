#ifndef PERS_SEG_TREE
#define PERS_SEG_TREE

#include <array>
#include <cassert>
#include <vector>

/**
 * Zero-indexed persistent segment tree
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 */

template <class T>
class persegtree {
   private:
    struct node {
        node* left = nullptr;
        node* right = nullptr;
        T val;

        node() = default;
        node(const T& x) : val(x) {}
    };

    int n;
    using nodeptr = node*;
    std::vector<nodeptr> vers;
    // q * log n
    constexpr static size_t MAX_NODES = 3e5 * 19 + 228;
    size_t _nodes_cnt = 0;
    std::array<node, MAX_NODES> _storage;

    // Create a node
    nodeptr create(const T& val = nullval) {
        _storage[_nodes_cnt] = node(val);
        assert(_nodes_cnt < MAX_NODES);
        return &_storage[_nodes_cnt++];
    }

    nodeptr copy(const nodeptr& u) {
        _storage[_nodes_cnt] = node(*u);
        assert(_nodes_cnt < MAX_NODES);
        return &_storage[_nodes_cnt++];
    }

    nodeptr build(int lo, int hi, const std::vector<T>& a) {
        nodeptr u = create();
        if (lo + 1 == hi) {
            u->val = a[lo];
            return u;
        }
        int mid = (lo + hi) / 2;
        u->left = build(lo, mid, a);
        u->right = build(mid, hi, a);
        u->val = f(u->left->val, u->right->val);
        return u;
    }

    T get_val(const nodeptr& u) { return u == nullptr ? nullval : u->val; }

   public:
    constexpr static T nullval = 0;

    T f(const T& x, const T& y) { return x + y; }

    persegtree(const std::vector<T>& a) : n(a.size()) { vers.push_back(build(0, n, a)); }

    void set(int v, int p, const T& x) { vers.push_back(set(vers[v], 0, n, p, x)); }

    nodeptr set(nodeptr u, int lo, int hi, int p, const T& x) {
        u = copy(u);
        if (lo + 1 == hi) {
            u->val = x;
            return u;
        }
        int mid = (lo + hi) / 2;
        if (p < mid)
            u->left = set(u->left, lo, mid, p, x);
        else
            u->right = set(u->right, mid, hi, p, x);
        u->val = f(get_val(u->left), get_val(u->right));
        return u;
    }

    T get(int v, int l, int r) { return get(vers[v], 0, n, l, r); }

    T get(nodeptr u, int lo, int hi, int l, int r) {
        if (u == nullptr || r <= lo || hi <= l) return T(nullval);
        if (l <= lo && hi <= r) return u->val;
        int mid = (lo + hi) / 2;
        return f(get(u->left, lo, mid, l, r), get(u->right, mid, hi, l, r));
    }
};

#endif /* PERS_SEG_TREE */
