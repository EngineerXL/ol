#ifndef LAZY_SEGMENT_TREE_FAST
#define LAZY_SEGMENT_TREE_FAST

#include <algorithm>
#include <limits>
#include <vector>

/**
 * Zero-indexed segment tree
 * max on segment, add val on segment
 * Intervals is expected to be [l, r)
 * 0 <= l < r <= n
 * T is node type, D is delay type
 * The constant factor is lower, than at recursive version
 */
template <class T, class D>
class lazy_segment_tree_t {
   private:
    int n, h;
    std::vector<T> tr;
    std::vector<D> delay;

    // Recusrive segment tree works well with l < 0 or r > n,
    // but non-recursive does not, so we should clamp query borders
    void clamp(int& pos) { pos = std::min(std::max(0, pos), n); }

    void build(const std::vector<T>& vec) {
        for (int i = 0; i < n; ++i) tr[i + n] = vec[i];
        for (int id = n - 1; id >= 0; --id) tr[id] = f(tr[id * 2 + 0], tr[id * 2 + 1]);
    }

    void apply_to_node(int pos, const D& val) {
        apply_to_val(tr[pos], val);
        if (pos < n) apply_to_del(delay[pos], val);
    }

    void push(int pos) {
        if (delay[pos] == nulldel) return;
        apply_to_node(pos * 2 + 0, delay[pos]);
        apply_to_node(pos * 2 + 1, delay[pos]);
        delay[pos] = nulldel;
    }

    // Apply modifications on route from root to pos node
    void apply_from_root(int pos) {
        pos += n;
        for (int d = h; d > 0; --d) push(pos >> d);
    }

    // Update node values on route from pos node to root
    void apply_from_leaf(int pos) {
        for (pos += n; pos > 1; pos /= 2) {
            if (pos & 1)
                tr[pos / 2] = f(tr[pos ^ 1], tr[pos]);
            else
                tr[pos / 2] = f(tr[pos], tr[pos ^ 1]);
            if (delay[pos / 2] != nulldel) apply_to_val(tr[pos / 2], delay[pos / 2]);
        }
    }

   public:
    T nullval = std::numeric_limits<T>::min();

    // Function on inverval
    T f(const T& x, const T& y) { return std::max(x, y); }

    constexpr static D nulldel = 0;

    // Apply delayed value to interval
    void apply_to_val(T& x, const D& y) { x += y; }

    // Update delayed value
    void apply_to_del(D& x, const D& y) { x += y; }

    lazy_segment_tree_t(int n)
        : n(n), h(sizeof(int) * 8 - __builtin_clz(n)), tr(2 * n), delay(n, nulldel) {}

    lazy_segment_tree_t(const std::vector<T>& vec) : lazy_segment_tree_t(vec.size()) { build(vec); }

    lazy_segment_tree_t(int n, const T& x) : lazy_segment_tree_t(std::vector<T>(n, x)) {}

    // Get f of elements on segment [l, r)
    T get(int l, int r) {
        clamp(l), clamp(r);
        if (l >= r) return nullval;
        apply_from_root(l), apply_from_root(r - 1);
        T resl = nullval, resr = nullval;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) resl = f(resl, tr[l++]);
            if (r & 1) resr = f(tr[--r], resr);
        }
        return f(resl, resr);
    }

    // Modify value on segment [l, r) by val
    void upd(int l, int r, const D& val) {
        clamp(l), clamp(r);
        if (l >= r) return;
        int l0 = l, r0 = r;
        apply_from_root(l), apply_from_root(r - 1);
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l & 1) apply_to_node(l++, val);
            if (r & 1) apply_to_node(--r, val);
        }
        apply_from_leaf(l0), apply_from_leaf(r0 - 1);
    }

    // Set value at position pos to val
    void set(int pos, const T& val) {
        apply_from_root(pos);
        tr[pos + n] = val;
        apply_from_leaf(pos);
    }
};

#endif /* LAZY_SEGMENT_TREE_FAST */
