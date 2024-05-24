#ifndef DIGITAL_TRIE_HPP
#define DIGITAL_TRIE_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

template <class T, int B>
class digital_trie_t {
   private:
    struct node_t {
        std::array<int, 2> go;
        int end, dp;

        node_t() : go({-1, -1}), end(0), dp(0) {}
    };

    std::vector<node_t> data;

    /* Returns index of new node */
    int create_node() {
        data.push_back(node_t());
        return data.size() - 1;
    }

    bool can_go(int u, bool bit) {
        return data[u].go[bit] != -1 && data[go(u, bit)].dp > 0;
    }

    int go(int u, bool bit) { return data[u].go[bit]; }

    bool get_bit(int x, int pos) { return x & (1 << pos); }

    const static int TAB_SIZE = 4;

    void print(int id, std::ostream& out, int h) {
        for (bool b : {0, 1}) {
            if (!can_go(id, b)) {
                continue;
            }
            for (int i = 0; i < TAB_SIZE * (h - 1); ++i) {
                out << ' ';
            }
            out << "|-> ";
            int next = go(id, b);
            out << '{' << b << ", dp =  " << data[next].dp
                << ", end = " << data[next].end << "}\n";
            print(next, out, h + 1);
        }
    }

   public:
    digital_trie_t() { create_node(); }

    void insert(int x) {
        int u = 0;
        for (int i = B - 1; i--;) {
            bool b = get_bit(x, i);
            if (!can_go(u, b)) {
                data[u].go[b] = create_node();
            }
            ++data[u].dp;
            u = go(u, b);
        }
        ++data[u].dp;
        ++data[u].end;
    }

    void erase(int x) {
        int u = 0;
        for (int i = B - 1; i--;) {
            bool b = get_bit(x, i);
            --data[u].dp;
            u = go(u, b);
        }
        --data[u].dp;
        --data[u].end;
    }

    int count(int x) {
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

    bool find(int x) { return count(x); }

    int descend(int x) {
        int u = 0, res = 0;
        for (int i = B - 1; i--;) {
            bool b = !get_bit(x, i);
            if (can_go(u, b)) {
                res |= (1 << i);
                u = go(u, b);
            } else if (can_go(u, !b)) {
                u = go(u, !b);
            } else {
                break;
            }
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, digital_trie_t& tr) {
        out << "root\n";
        tr.print(0, out, 1);
        return out;
    }

    ~digital_trie_t() = default;
};

using trie32_t = digital_trie_t<int32_t, 32>;
using trie64_t = digital_trie_t<int64_t, 64>;

#endif /* DIGITAL_TRIE_HPP */
