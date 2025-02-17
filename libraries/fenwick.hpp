#ifndef FENWICK_HPP
#define FENWICK_HPP

#include <vector>

// Binary indexed tree
// aka Fenwick tree
template <class T>
struct fenwick_t {
    int n;
    std::vector<T> data;

    fenwick_t(int n) : n(n), data(n) {}

    // Get the sum on the prefix of lenght x
    // get(3) -> a[0] + a[1] + a[2]
    T get(int x) {
        T res = T();
        for (; x > 0; x &= x - 1) {
            res += data[x - 1];
        }
        return res;
    }

    // Add val to x-th element (0-indexed)
    // a[x] += val
    void add(int x, const T& val) {
        for (; x < n; x |= x + 1) {
            data[x] += val;
        }
    }
};

#endif /* FENWICK_HPP */
