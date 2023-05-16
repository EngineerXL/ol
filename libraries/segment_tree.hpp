#ifndef SEGMENT_TREE
#define SEGMENT_TREE

#include <vector>

template <class T>
class segment_tree_t {
   private:
    size_t _size;
    std::vector<T> _data;

   public:
    segment_tree_t(const size_t& n) : _size(n), _data(4 * n) {}

    segment_tree_t(const std::vector<T>& vec)
        : _size(vec.size()), _data(4 * vec.size()) {
        build(vec, 1, 1, _size);
    }

    void build(const std::vector<T>& vec, size_t id, size_t l, size_t r) {
        if (l >= r) {
            _data[id] = vec[l - 1];
            return;
        }
        size_t m = (l + r) / 2;
        build(vec, id * 2, l, m);
        build(vec, id * 2 + 1, m + 1, r);
        _data[id] = _data[id * 2] + _data[id * 2 + 1];
    }

    ~segment_tree_t() = default;

    T get(size_t ql, size_t qr) { return get(1, ql, qr, 1, _size); }

    T get(size_t id, size_t ql, size_t qr, size_t l, size_t r) {
        if (ql <= l and r <= qr) {
            return _data[id];
        }
        size_t m = (l + r) / 2;
        if (qr <= m) {
            return get(id * 2, ql, qr, l, m);
        }
        if (ql > m) {
            return get(id * 2 + 1, ql, qr, m + 1, r);
        }
        return get(id * 2, ql, qr, l, m) + get(id * 2 + 1, ql, qr, m + 1, r);
    }

    void set(size_t pos, const T& elem) { set(1, 1, _size, pos, elem); }

    void set(size_t id, size_t l, size_t r, size_t pos, const T& elem) {
        if (l >= r) {
            _data[id] = elem;
            return;
        }
        size_t m = (l + r) / 2;
        if (pos <= m) {
            set(id * 2, l, m, pos, elem);
        } else {
            set(id * 2 + 1, m + 1, r, pos, elem);
        }
        _data[id] = _data[id * 2] + _data[id * 2 + 1];
    }
};

#endif /* SEGMENT_TREE */
