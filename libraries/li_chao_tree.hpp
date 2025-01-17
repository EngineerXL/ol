#ifndef LI_CHAO_TREE
#define LI_CHAO_TREE

#include <algorithm>
#include <array>
#include <vector>

using ll = long long;
static const ll INF = 1e18;

// This struct could be modified
struct line_t {
    ll k, b;
    line_t(ll k = 0, ll b = INF) : k(k), b(b) {}
    ll operator()(ll x) { return k * x + b; }
};

struct node_t {
    node_t* l = nullptr;
    node_t* r = nullptr;
    line_t f;
};

// Use vector or array if delete is not needed to speed up
using ptr = node_t*;
// std::array<node_t, N> storage;
// std::vector<node_t> storage;
ptr create() { return new node_t(); }

// Li-Chao tree finds minimum f(x) for given x
// Among all added f(x)
struct li_chao_tree_t {
    ll minl, maxr;
    ptr root;

    li_chao_tree_t(ll minl, ll maxr) : minl(minl), maxr(maxr) {
        root = create();
    }

    void add(line_t f) { add(root, minl, maxr, f); }

    void add(ptr u, ll lo, ll hi, line_t g) {
        line_t f = u->f;
        if (lo + 1 == hi) {
            u->f = g(lo) < f(lo) ? g : f;
            return;
        }
        if (!u->l) {
            u->l = create(), u->r = create();
            u->l->f = u->r->f = f;
        }
        ll mid = (lo + hi) / 2;
        bool condl = g(lo) < f(lo), condm = g(mid) < f(mid);
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

    ll get(ll x) { return get(root, minl, maxr, x); }

    ll get(ptr u, ll lo, ll hi, ll x) {
        if (!u) return INF;
        if (lo + 1 == hi) return u->f(x);
        ll mid = (lo + hi) / 2;
        if (x < mid) {
            return std::min(u->f(x), get(u->l, lo, mid, x));
        } else {
            return std::min(u->f(x), get(u->r, mid, hi, x));
        }
    }

    // Tree could be used after clear
    void clear() {
        clear(root);
        *root = node_t();
    }

    void clear(ptr u) {
        if (!u->l) return;
        clear(u->l);
        clear(u->r);
        delete u->l;
        delete u->r;
    }
};

#endif /* LI_CHAO_TREE */
