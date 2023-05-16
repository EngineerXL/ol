#ifndef LAZY_SEGMENT_TREE
#define LAZY_SEGMENT_TREE

template <class T>
class lazy_segment_tree_t {
   private:
    size_t _size;
    std::vector<T> _data;
    std::vector<T> _delay;

   public:
    lazy_segment_tree_t(const size_t& n)
        : _size(n), _data(4 * n), _delay(4 * n) {}

    lazy_segment_tree_t(const std::vector<T>& vec)
        : lazy_segment_tree_t(vec.size()) {
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
        _data[id] = std::min(_data[id * 2], _data[id * 2 + 1]);
    }

    ~lazy_segment_tree_t() = default;

    void push(size_t id, size_t l, size_t r) {
        if (_delay[id] == 0) {
            return;
        }
        _data[id] += _delay[id];
        if (id * 2 < _data.size()) {
            _delay[id * 2] += _delay[id];
        }
        if (id * 2 + 1 < _data.size()) {
            _delay[id * 2 + 1] += _delay[id];
        }
        _delay[id] = 0;
    }

    T get(size_t ql, size_t qr) { return get(1, ql, qr, 1, _size); }

    T get(size_t id, size_t ql, size_t qr, size_t l, size_t r) {
        push(id, l, r);
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
        return std::min(get(id * 2, ql, qr, l, m),
                        get(id * 2 + 1, ql, qr, m + 1, r));
    }

    void add(size_t ql, size_t qr, const T& val) {
        add(1, ql, qr, 1, _size, val);
    }

    void add(size_t id, size_t ql, size_t qr, size_t l, size_t r,
             const T& val) {
        push(id, l, r);
        if (qr < l or r < ql) {
            return;
        }
        if (ql <= l and r <= qr) {
            _delay[id] += val;
            push(id, l, r);
            return;
        }
        size_t m = (l + r) / 2;
        add(id * 2, ql, qr, l, m, val);
        add(id * 2 + 1, ql, qr, m + 1, r, val);
        _data[id] = std::min(_data[id * 2], _data[id * 2 + 1]);
    }
};

#endif /* LAZY_SEGMENT_TREE */
