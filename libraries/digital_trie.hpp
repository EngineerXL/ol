#ifndef DIGITAL_TRIE_HPP
#define DIGITAL_TRIE_HPP

#include <array>
#include <cstdint>
#include <vector>

template <class T, int B>
struct digital_trie_t {
   private:
    struct node_t {
        std::array<int, 2> go;
        int dp;

        node_t() : go({-1, -1}), dp(0) {}
    };

    std::vector<node_t> data;

    /* Returns index of new node */
    int create_node() {
        data.emplace_back();
        return data.size() - 1;
    }

    bool can_go(int u, bool bit) {
        return go(u, bit) != -1 && dp(go(u, bit)) > 0;
    }

    int& go(int u, bool bit) { return data[u].go[bit]; }

    int& dp(int u) { return data[u].dp; }

   public:
    const int SUM_S = (2e5 + 200) * B;

    digital_trie_t() {
        data.reserve(SUM_S);
        create_node();
    }

    bool get_bit(T x, int pos) { return x & (1ll << pos); }

    void insert(T x) {
        int u = 0;
        for (int i = B - 1; i--;) {
            bool b = get_bit(x, i);
            if (!can_go(u, b)) {
                go(u, b) = create_node();
            }
            ++dp(u);
            u = go(u, b);
        }
        ++dp(u);
    }

    void erase(T x) {
        int u = 0;
        for (int i = B - 1; i--;) {
            bool b = get_bit(x, i);
            --dp(u);
            u = go(u, b);
        }
        --dp(u);
    }

    int count(T x) {
        int u = 0;
        for (int i = B - 1; i--;) {
            bool b = get_bit(x, i);
            if (!can_go(u, b)) {
                return 0;
            }
            u = go(u, b);
        }
        return data[u].end;
    }

    T descend(T x) {
        int u = 0;
        T res = 0;
        for (int i = B - 1; i--;) {
            bool b = !get_bit(x, i);
            if (can_go(u, b)) {
                res |= (1ll << i);
                u = go(u, b);
            } else if (can_go(u, !b)) {
                u = go(u, !b);
            } else {
                break;
            }
        }
        return res;
    }
};

using trie32_t = digital_trie_t<int32_t, 32>;
using trie64_t = digital_trie_t<int64_t, 64>;

#endif /* DIGITAL_TRIE_HPP */
