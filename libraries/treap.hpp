#ifndef TREAP_HPP
#define TREAP_HPP

#include <array>
#include <cassert>
#include <random>

using treap_key_type = uint32_t;
using treap_val_type = int;

// Function on segment
treap_val_type f(const treap_val_type& lhs, const treap_val_type& rhs) {
    return lhs + rhs;
}

// Cartesian tree node
// x - size of the subtree
// Change treap_val_type and f for your needs
// Set MAX_NODES to constrains of your problem
struct node_t {
    node_t* left = nullptr;
    node_t* right = nullptr;
    treap_key_type x;
    treap_val_type data, seg;

    node_t() = default;
    node_t(const treap_val_type& val) : x(1), data(val), seg(val) {}
};

const size_t MAX_NODES = 5e5 + 228;
size_t _nodes_cnt = 0;
std::array<node_t, MAX_NODES> _storage;
std::mt19937 _treap_rng(1000 - 7);

// Cartesian tree node pointer
using treap = node_t*;

// Create a node
treap create(const treap_val_type& val) {
    _storage[_nodes_cnt] = node_t(val);
    assert(_nodes_cnt < MAX_NODES);
    return &_storage[_nodes_cnt++];
}

treap_key_type get_x(treap t) { return t ? t->x : 0; }

treap_val_type get_seg(treap t) { return t ? t->seg : treap_val_type(); }

void update(treap t) {
    if (t == nullptr) return;
    t->x = 1 + get_x(t->left) + get_x(t->right);
    t->seg = f(get_seg(t->left), f(t->data, get_seg(t->right)));
}

// Splits t into l and r, invalidates pointer
// l will contain x elements
void split(treap t, treap_key_type x, treap& l, treap& r) {
    if (t == nullptr) {
        l = r = nullptr;
        return;
    }
    if (x <= get_x(t->left)) {
        split(t->left, x, l, t->left);
        r = t;
    } else {
        split(t->right, x - 1 - get_x(t->left), t->right, r);
        l = t;
    }
    update(t);
}

// Merges two treaps l and r, invalidates pointers
treap merge(treap l, treap r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;
    if (_treap_rng() % (get_x(l) + get_x(r)) < get_x(l)) {
        l->right = merge(l->right, r);
        update(l);
        return l;
    } else {
        r->left = merge(l, r->left);
        update(r);
        return r;
    }
}

#endif /* TREAP_HPP */
