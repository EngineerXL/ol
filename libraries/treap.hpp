#ifndef TREAP_HPP
#define TREAP_HPP

#include <chrono>
#include <iostream>
#include <random>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

class treap_t {
    static const int64_t INF = 1e18;
    static const int TAB_SIZE = 4;

    struct treap_node_t {
        treap_node_t *_left;
        treap_node_t *_right;
        int _key;
        int64_t _priority;

        int64_t _value;
        int64_t _seg;
        bool _reverse;
        int64_t _delay;

        treap_node_t(int key, int64_t value) {
            _left = nullptr;
            _right = nullptr;
            _key = key;
            _priority = rng();
            _value = value;
            _seg = value;
            _reverse = false;
            _delay = 0;
        }

        ~treap_node_t() {
            delete _left;
            delete _right;
        }
    };

    using treap_ptr = treap_node_t *;

    treap_ptr root;

    int get_key(treap_ptr t) {
        if (t != nullptr) {
            return t->_key;
        } else {
            return 0;
        }
    }

    void reverse(treap_ptr t) {
        if (t != nullptr) {
            t->_reverse = t->_reverse ^ 1;
        }
    }

    /* Change this */
    int64_t get_value(treap_ptr t) {
        if (t != nullptr) {
            push(t);
            return t->_value;
        } else {
            return -INF;
        }
    }

    /* Change this */
    int64_t get_seg(treap_ptr t) {
        if (t != nullptr) {
            push(t);
            return t->_seg;
        } else {
            return -INF;
        }
    }

    /* Change this */
    void update(treap_ptr t, int64_t x) {
        if (t != nullptr) {
            t->_delay += x;
        }
    }

    /* Change this */
    void push(treap_ptr t) {
        if (t != nullptr) {
            if (t->_delay != 0) {
                update(t->_left, t->_delay);
                update(t->_right, t->_delay);
                t->_value += t->_delay;
                t->_seg += t->_delay;
                t->_delay = 0;
            }
            if (t->_reverse) {
                std::swap(t->_left, t->_right);
                reverse(t->_left);
                reverse(t->_right);
                t->_reverse = false;
            }
        }
    }

    /* Change this */
    void update_node(treap_ptr t) {
        if (t != nullptr) {
            t->_key = 1 + get_key(t->_left) + get_key(t->_right);
            t->_seg = std::max(get_seg(t->_left), get_seg(t->_right));
            t->_seg = std::max(t->_seg, t->_value);
        }
    }

    void split(treap_ptr t, int key, treap_ptr &l, treap_ptr &r) {
        if (t == nullptr) {
            l = nullptr;
            r = nullptr;
            return;
        }
        push(t);
        int key_left_subtree = get_key(t->_left);
        if (key > key_left_subtree + 1) {
            split(t->_right, key - key_left_subtree - 1, t->_right, r);
            l = t;
        } else {
            split(t->_left, key, l, t->_left);
            r = t;
        }
        update_node(t);
    }

    treap_ptr merge(treap_ptr l, treap_ptr r) {
        if (l == nullptr) {
            return r;
        }
        if (r == nullptr) {
            return l;
        }
        if (l->_priority > r->_priority) {
            push(l);
            l->_right = merge(l->_right, r);
            update_node(l);
            return l;
        } else {
            push(r);
            r->_left = merge(l, r->_left);
            update_node(r);
            return r;
        }
    }

    friend std::ostream &operator<<(std::ostream &out, treap_t t) {
        print_treap(out, t.root, 0);
        return out;
    }

    friend std::ostream &operator<<(std::ostream &out, treap_ptr t) {
        print_treap(out, t, 0);
        return out;
    }

    friend void print_treap(std::ostream &out, treap_ptr t, int h) {
        if (t != nullptr) {
            print_treap(out, t->_left, h + 1);
            for (int i = 0; i < TAB_SIZE * h; ++i) {
                out << " ";
            }
            out << "{ key = " << t->_key << ", val = " << t->_value
                << ", seg = " << t->_seg << "}\n";
            print_treap(out, t->_right, h + 1);
        }
    }

   public:
    treap_t() { root = nullptr; }

    void set(int key, int64_t value) {
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key + 1, m, r);
        split(m, key, l, m);
        m->_value = value;
        m->_seg = value;
        root = merge(merge(l, m), r);
    }

    int64_t get(int key_l, int key_r) {
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key_r + 1, m, r);
        split(m, key_l, l, m);
        int64_t ans = get_seg(m);
        root = merge(merge(l, m), r);
        return ans;
    }

    void update(int key_l, int key_r, int64_t x) {
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key_r + 1, m, r);
        split(m, key_l, l, m);
        update(m, x);
        root = merge(merge(l, m), r);
    }

    void reverse(int key_l, int key_r) {
        treap_ptr l = nullptr;
        treap_ptr m = nullptr;
        treap_ptr r = nullptr;
        split(root, key_r + 1, m, r);
        split(m, key_l, l, m);
        reverse(m);
        root = merge(merge(l, m), r);
    }

    void push_back(int64_t value) {
        treap_ptr m = new treap_node_t(1, value);
        root = merge(root, m);
    }

    void push_front(int64_t value) {
        treap_ptr m = new treap_node_t(1, value);
        root = merge(m, root);
    }

    ~treap_t() { delete root; }
};

#endif /* TREAP_HPP */
