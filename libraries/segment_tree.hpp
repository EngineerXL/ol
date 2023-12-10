#ifndef SEGMENT_TREE
#define SEGMENT_TREE

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
    int n;

    using vt = std::vector<T>;
    vt tr;

    void build(const vt& vec, int id, int lo, int hi) {
        if (lo + 1 == hi) {
            tr[id] = vec[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(vec, id * 2 + 1, lo, mid);
        build(vec, id * 2 + 2, mid, hi);
        tr[id] = tr[id * 2 + 1] + tr[id * 2 + 2];
    }

    T get(int id, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) {
            return T();
        }
        if (l <= lo && hi <= r) {
            return tr[id];
        }
        int mid = (lo + hi) / 2;
        return get(id * 2 + 1, lo, mid, l, r) + get(id * 2 + 2, mid, hi, l, r);
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
        tr[id] = tr[id * 2 + 1] + tr[id * 2 + 2];
    }

   public:
    segment_tree_t(int _n) : n(_n), tr(4 * n) {}

    segment_tree_t(const vt& vec) : segment_tree_t(vec.size()) {
        build(vec, 0, 0, n);
    }

    T get(int l, int r) { return get(0, 0, n, l, r); }

    void set(int pos, const T& elem) { set(0, 0, n, pos, elem); }
};

#endif /* SEGMENT_TREE */