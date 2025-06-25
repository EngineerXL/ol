#ifndef LAZY_SEGMENT_TREE
#define LAZY_SEGMENT_TREE

#include <algorithm>
#include <limits>
#include <vector>

/**
 * Zero-indexed segment tree
 * max on segment, add val on segment
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 */

template <class T>
class lazy_segment_tree_t {
   private:
    using vt = std::vector<T>;
    int n;
    vt tr;
    vt delay;

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

    void push(int id, int lo, int hi) {
        if (delay[id] == 0) {
            return;
        }
        tr[id] += delay[id];
        if (lo + 1 < hi) {
            delay[id * 2 + 1] += delay[id];
            delay[id * 2 + 2] += delay[id];
        }
        delay[id] = 0;
    }

    T get(int id, int lo, int hi, int l, int r) {
        push(id, lo, hi);
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

    void upd(int id, int lo, int hi, int l, int r, const T& val) {
        push(id, lo, hi);
        if (r <= lo || hi <= l) {
            return;
        }
        if (l <= lo && hi <= r) {
            delay[id] += val;
            push(id, lo, hi);
            return;
        }
        int mid = (lo + hi) / 2;
        upd(id * 2 + 1, lo, mid, l, r, val);
        upd(id * 2 + 2, mid, hi, l, r, val);
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
    }

   public:
    T nullval = std::numeric_limits<T>::min();

    T f(const T& x, const T& y) { return std::max(x, y); }

    lazy_segment_tree_t(int n) : n(n), tr(4 * n), delay(4 * n) {}

    lazy_segment_tree_t(const vt& vec) : lazy_segment_tree_t(vec.size()) {
        build(vec, 0, 0, n);
    }

    lazy_segment_tree_t(int n, const T& x)
        : lazy_segment_tree_t(std::vector<T>(n, x)) {}

    T get(int l, int r) { return get(0, 0, n, l, r); }

    void upd(int l, int r, const T& val) { upd(0, 0, n, l, r, val); }
};

#endif /* LAZY_SEGMENT_TREE */
