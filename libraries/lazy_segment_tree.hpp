#ifndef LAZY_SEGMENT_TREE
#define LAZY_SEGMENT_TREE

#include <vector>

/**
 * Zero-indexed segment tree
 * min and add on segment
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 */

template <class T>
class lazy_segment_tree_t {
   private:
    int n;

    using vt = std::vector<T>;
    vt tr;
    vt delay;

    const T INF = 1e18;

    T f(const T& x, const T& y) { return std::min(x, y); }

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
            return INF;
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
    lazy_segment_tree_t(int _n) : n(_n), tr(4 * n), delay(4 * n) {}

    lazy_segment_tree_t(const vt& vec) : lazy_segment_tree_t(vec.size()) {
        build(vec, 0, 0, n);
    }

    T get(int l, int r) { return get(0, 0, n, l, r); }

    void upd(int l, int r, const T& val) { upd(0, 0, n, l, r, val); }
};

#endif /* LAZY_SEGMENT_TREE */
