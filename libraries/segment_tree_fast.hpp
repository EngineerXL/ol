#ifndef SEGMENT_TREE_FAST
#define SEGMENT_TREE_FAST

#include <algorithm>
#include <limits>
#include <vector>

/**
 * Zero-indexed non-recursive segment tree
 * max on segment, set val at pos
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 * The constant factor is lower, than at recursive version
 */

#include <iostream>

template <class T>
class segment_tree_t {
   private:
    using vt = std::vector<T>;
    int n;
    vt tr;

    void build(const vt& vec) {
        for (int i = 0; i < n; ++i) tr[i + n] = vec[i];
        for (int id = n - 1; id >= 0; --id) tr[id] = f(tr[id * 2 + 0], tr[id * 2 + 1]);
    }

   public:
    T nullval = std::numeric_limits<T>::min();

    T f(const T& x, const T& y) { return std::max(x, y); }

    segment_tree_t(int n) : n(n), tr(2 * n) {}

    segment_tree_t(const vt& vec) : segment_tree_t(vec.size()) { build(vec); }

    segment_tree_t(int n, const T& x) : segment_tree_t(std::vector<T>(n, x)) {}

    // Get f of elements on segment [l, r)
    T get(int l, int r) {
        T resl = nullval, resr = nullval;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) resl = f(resl, tr[l++]);
            if (r & 1) resr = f(tr[--r], resr);
        }
        return f(resl, resr);
    }

    // Set value at position pos to val
    void set(int pos, const T& val) {
        tr[pos += n] = val;
        for (; pos > 1; pos /= 2) tr[pos / 2] = f(tr[pos], tr[pos ^ 1]);
    }
};

#endif /* SEGMENT_TREE_FAST */