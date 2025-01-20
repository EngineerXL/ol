#ifndef LI_CHAO_TREE_CONV
#define LI_CHAO_TREE_CONV

#include <algorithm>
#include <vector>

using ll = long long;
const ll INF = 1e18;

// Li-Chao tree finds minimum f(x) for given x
// Among all added f(x)
struct li_chao_tree_conv_t {
    std::vector<ll> a, b;
    int n, m;

    // B is convex, so A_i + B is also convex
    // and we can store them in the tree
    ll calc(int i, int x) {
        if (i < 1) return INF;
        return i + 1 <= x && x <= i + m ? a[i - 1] + b[x - i - 1] : INF;
    }

    struct node_t {
        node_t* l = nullptr;
        node_t* r = nullptr;
        int f;
    };

    using ptr = node_t*;
    std::vector<node_t> storage;
    ptr create(int i = 0) {
        storage.emplace_back();
        storage.back().f = i;
        return &storage.back();
    }

    ptr root;

    li_chao_tree_conv_t(const std::vector<ll>& a, const std::vector<ll>& b)
        : a(a), b(b), n(a.size()), m(b.size()) {
        storage.reserve(4 * (n + m));
        root = create();
    }

    void add(int f) { add(root, 2, n + m + 1, f); }

    void add(ptr u, ll lo, ll hi, int g) {
        int f = u->f;
        if (lo + 1 == hi) {
            u->f = calc(g, lo) < calc(f, lo) ? g : f;
            return;
        }
        if (!u->l) {
            u->l = create(), u->r = create();
            u->l->f = u->r->f = f;
        }
        ll mid = (lo + hi) / 2;
        bool condl = calc(g, lo) < calc(f, lo),
             condm = calc(g, mid) < calc(f, mid);
        if (condm) {
            // Don't lose the old function
            std::swap(u->f, g);
        }
        if (condl != condm) {
            add(u->l, lo, mid, g);
        } else {
            add(u->r, mid, hi, g);
        }
    }

    ll get(ll x) { return get(root, 2, n + m + 1, x); }

    ll get(ptr u, ll lo, ll hi, ll x) {
        if (!u) return INF;
        if (lo + 1 == hi) return calc(u->f, x);
        ll mid = (lo + hi) / 2;
        if (x < mid) {
            return std::min(calc(u->f, x), get(u->l, lo, mid, x));
        } else {
            return std::min(calc(u->f, x), get(u->r, mid, hi, x));
        }
    }
};

// Computes C_i = min(A_j + B_k)
// where j + k = i (1-indexed)
// B should be convex
// C_1 = inf
// To calculate conv_max
// use -conv_min(-a, -b)
std::vector<ll> conv_min(const std::vector<ll>& a, const std::vector<ll>& b) {
    int n = a.size(), m = b.size();
    li_chao_tree_conv_t lt(a, b);
    for (int i = 1; i <= n; ++i) lt.add(i);
    std::vector<ll> c(n + m, INF);
    for (int i = 2; i <= n + m; ++i) c[i - 1] = lt.get(i);
    return c;
}

#endif /* LI_CHAO_TREE_CONV */
