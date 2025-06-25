#ifndef SEGMENT_TREE
#define SEGMENT_TREE

#include <algorithm>
#include <limits>
#include <vector>

/**
 * Zero-indexed segment tree
 * T should have operator+
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 */

template <class T>
class segment_tree_t {
   private:
    using vt = std::vector<T>;
    int n;
    vt tr;

    void build(const vt& vec, int id, int lo, int hi) {
        if (lo + 1 == hi) {
            tr[id] = vec[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(vec, id * 2 + 1, lo, mid);
        build(vec, id * 2 + 2, mid, hi);
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
    }

    T get(int id, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) {
            return nullval;
        }
        if (l <= lo && hi <= r) {
            return tr[id];
        }
        int mid = (lo + hi) / 2;
        return f(get(id * 2 + 1, lo, mid, l, r),
                 get(id * 2 + 2, mid, hi, l, r));
    }

    void set(int id, int lo, int hi, int pos, const T& elem) {
        if (lo + 1 == hi) {
            tr[id] = elem;
            return;
        }
        int mid = (lo + hi) / 2;
        if (pos < mid) {
            set(id * 2 + 1, lo, mid, pos, elem);
        } else {
            set(id * 2 + 2, mid, hi, pos, elem);
        }
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
    }

   public:
    T nullval = std::numeric_limits<T>::min();

    T f(const T& x, const T& y) { return std::max(x, y); }

    segment_tree_t(int n) : n(n), tr(4 * n) {}

    segment_tree_t(const vt& vec) : segment_tree_t(vec.size()) {
        build(vec, 0, 0, n);
    }

    segment_tree_t(int n, const T& x) : segment_tree_t(std::vector<T>(n, x)) {}

    T get(int l, int r) { return get(0, 0, n, l, r); }

    void set(int pos, const T& elem) { set(0, 0, n, pos, elem); }
};

#endif /* SEGMENT_TREE */