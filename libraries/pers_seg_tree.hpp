#ifndef PERS_SEG_TREE
#define PERS_SEG_TREE

#include <vector>

template <class T>
struct pers_seg_tree {
    struct tree_node {
        tree_node* left = nullptr;
        tree_node* right = nullptr;
        T val = 0;
    };

    using node_ptr = tree_node*;

    int n;
    node_ptr root;
    std::vector<node_ptr> vers;

    void build(const std::vector<T>& v) {
        n = v.size();
        root = build(v, 0, n - 1);
        vers.push_back(root);
    }

    node_ptr build(const std::vector<T>& v, int l, int r) {
        node_ptr u = new tree_node();
        if (l == r) {
            u->val = v[l];
        } else {
            int m = (l + r) / 2;
            u->left = build(v, l, m);
            u->right = build(v, m + 1, r);
            u->val = u->left->val + u->right->val;
        }
        return u;
    }

    node_ptr copy(const node_ptr u) {
        node_ptr v = new tree_node(*u);
        return v;
    }

    void set(int k, int x, const T& v) {
        root = set(vers[k], x, v, 0, n - 1);
        vers.push_back(root);
    }

    node_ptr set(node_ptr u, int x, const T& v, int lb, int rb) {
        u = copy(u);
        if (lb == rb) {
            u->val = v;
            return u;
        }
        int mb = (lb + rb) / 2;
        if (x > mb) {
            u->right = set(u->right, x, v, mb + 1, rb);
        } else {
            u->left = set(u->left, x, v, lb, mb);
        }
        u->val = u->left->val + u->right->val;
        return u;
    }

    T get(int k, int l, int r) { return get(vers[k], l, r, 0, n - 1); }

    T get(node_ptr u, int l, int r, int lb, int rb) {
        if (l <= lb and rb <= r) {
            return u->val;
        }
        int mb = (lb + rb) / 2;
        if (r <= mb) {
            return get(u->left, l, r, lb, mb);
        }
        if (l > mb) {
            return get(u->right, l, r, mb + 1, rb);
        }
        return get(u->left, l, r, lb, mb) + get(u->right, l, r, mb + 1, rb);
    }
};

#endif /* PERS_SEG_TREE */
