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
 * T is node type, D is delay type
 */
template <class T, class D>
class lazy_segment_tree_t {
   private:
    int n;
    std::vector<T> tr;
    std::vector<D> delay;

    void build(const std::vector<T>& vec, int id, int lo, int hi) {
        if (lo + 1 == hi) {
            tr[id] = vec[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(vec, id * 2 + 1, lo, mid);
        build(vec, id * 2 + 2, mid, hi);
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
    }

    void apply_to_node(int id, const D& val) {
        apply_to_val(tr[id], val);
        apply_to_del(delay[id], val);
    }

    void push(int id) {
        if (delay[id] == nulldel) return;
        apply_to_node(id * 2 + 1, delay[id]);
        apply_to_node(id * 2 + 2, delay[id]);
        delay[id] = nulldel;
    }

    T get(int id, int lo, int hi, int l, int r) {
        if (r <= lo || hi <= l) return nullval;
        if (l <= lo && hi <= r) return tr[id];
        push(id);
        int mid = (lo + hi) / 2;
        return f(get(id * 2 + 1, lo, mid, l, r), get(id * 2 + 2, mid, hi, l, r));
    }

    void upd(int id, int lo, int hi, int l, int r, const D& val) {
        if (r <= lo || hi <= l) return;
        if (l <= lo && hi <= r) {
            apply_to_node(id, val);
            return;
        }
        push(id);
        int mid = (lo + hi) / 2;
        upd(id * 2 + 1, lo, mid, l, r, val);
        upd(id * 2 + 2, mid, hi, l, r, val);
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
    }

    void set(int id, int lo, int hi, int pos, const T& val) {
        if (pos < lo || pos >= hi) return;
        if (lo + 1 == hi) {
            tr[id] = val;
            return;
        }
        push(id);
        int mid = (lo + hi) / 2;
        set(id * 2 + 1, lo, mid, pos, val);
        set(id * 2 + 2, mid, hi, pos, val);
        tr[id] = f(tr[id * 2 + 1], tr[id * 2 + 2]);
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

    lazy_segment_tree_t(int n) : n(n), tr(4 * n), delay(4 * n, nulldel) {}

    lazy_segment_tree_t(const std::vector<T>& vec) : lazy_segment_tree_t(vec.size()) {
        build(vec, 0, 0, n);
    }

    lazy_segment_tree_t(int n, const T& x) : lazy_segment_tree_t(std::vector<T>(n, x)) {}

    // Get f of elements on segment [l, r)
    T get(int l, int r) { return get(0, 0, n, l, r); }

    // Modify value on segment [l, r) by val
    void upd(int l, int r, const D& val) { upd(0, 0, n, l, r, val); }

    // Set value at position pos to val
    void set(int pos, const T& val) { set(0, 0, n, pos, val); }
};

#endif /* LAZY_SEGMENT_TREE */
