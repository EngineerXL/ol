#ifndef TRIE_HPP
#define TRIE_HPP

#include <map>
#include <string>
#include <vector>

struct trie_t {
    struct node_t {
        std::map<char, int> go;
        int end = 0;
        int dp = 0;
    };

    using item_t = std::pair<char, int>;

    std::vector<node_t> data;

    /* Returns index of new node */
    int create_node() {
        data.emplace_back();
        return data.size() - 1;
    }

    bool can_go(int u, char c) { return data[u].go.count(c); }

    int& go(int u, char c) { return data[u].go[c]; }

    int& dp(int u) { return data[u].dp; }

    int& end(int u) { return data[u].end; }

    trie_t() { create_node(); }

    void insert(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                go(u, c) = create_node();
            }
            ++dp(u);
            u = go(u, c);
        }
        ++dp(u);
        ++end(u);
    }

    void erase(const std::string& s) {
        int u = 0;
        for (char c : s) {
            --dp(u);
            u = go(u, c);
        }
        --dp(u);
        --end(u);
    }

    int count(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                return 0;
            }
            u = go(u, c);
        }
        return end(u);
    }
};

#endif /* TRIE_HPP */
