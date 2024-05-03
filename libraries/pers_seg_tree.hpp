#ifndef PERS_SEG_TREE
#define PERS_SEG_TREE

#include <vector>

/**
 * Zero-indexed persistent segment tree
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 */

template <class T>
struct persegtree {
    struct node {
        node* left;
        node* right;
        T val;

        node(const T& x) : left(nullptr), right(nullptr), val(x) {}

        node(const node& other)
            : left(other.left), right(other.right), val(other.val) {}
    };

    int n;
    using nodeptr = node*;
    std::vector<nodeptr> vers;

    persegtree(int n_) : n(n_) {}

    persegtree(const std::vector<T>& a) : persegtree(a.size()) {
        vers.push_back(build(0, n, a));
    }

    nodeptr build(int lo, int hi, const std::vector<T>& a) {
        nodeptr u = new node(0);
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

    T f(const T& x, const T& y) { return x + y; }

    T get_val(const nodeptr& u) { return u == nullptr ? 0 : u->val; }

    nodeptr copy(const nodeptr& u) { return new node(*u); }

    void set(int v, int p, const T& x) {
        vers.push_back(set(vers[v], 0, n, p, x));
    }

    nodeptr set(nodeptr u, int lo, int hi, int p, const T& x) {
        u = copy(u);
        if (lo + 1 == hi) {
            u->val = x;
            return u;
        }
        int mid = (lo + hi) / 2;
        if (p < mid) {
            u->left = set(u->left, lo, mid, p, x);
        } else {
            u->right = set(u->right, mid, hi, p, x);
        }
        u->val = f(get_val(u->left), get_val(u->right));
        return u;
    }

    T get(int v, int l, int r) { return get(vers[v], 0, n, l, r); }

    T get(nodeptr u, int lo, int hi, int l, int r) {
        if (u == nullptr || r <= lo || hi <= l) {
            return T(0);
        }
        if (l <= lo && hi <= r) {
            return u->val;
        }
        int mid = (lo + hi) / 2;
        return f(get(u->left, lo, mid, l, r), get(u->right, mid, hi, l, r));
    }
};

#endif /* PERS_SEG_TREE */
