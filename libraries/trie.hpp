#ifndef TRIE_HPP
#define TRIE_HPP

#include <iostream>
#include <map>
#include <vector>

class trie_t {
   private:
    struct node_t {
        std::map<char, int> go;
        int end = 0;
        int dp = 0;
    };

    using item_t = std::pair<char, int>;

    std::vector<node_t> data;

    /* Returns index of new node */
    int create_node() {
        data.push_back(node_t());
        return data.size() - 1;
    }

    bool can_go(int u, char c) { return data[u].go.count(c); }

    int go(int u, char c) { return data[u].go[c]; }

    const static int TAB_SIZE = 4;

    void print(int id, std::ostream& out, int h) {
        for (item_t elem : data[id].go) {
            for (int i = 0; i < TAB_SIZE * (h - 1); ++i) {
                out << ' ';
            }
            out << "|-> ";
            int next = elem.second;
            out << '{' << elem.first << ", dp =  " << data[next].dp
                << ", end = " << data[next].end << "}\n";
            print(next, out, h + 1);
        }
    }

   public:
    trie_t() { create_node(); }

    void insert(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                data[u].go[c] = create_node();
            }
            ++data[u].dp;
            u = go(u, c);
        }
        ++data[u].dp;
        ++data[u].end;
    }

    void erase(const std::string& s) {
        int u = 0;
        for (char c : s) {
            --data[u].dp;
            u = go(u, c);
        }
        --data[u].dp;
        --data[u].end;
    }

    bool find(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                return false;
            }
            u = go(u, c);
        }
        return data[u].end;
    }

    int count(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                return 0;
            }
            u = go(u, c);
        }
        return data[u].end;
    }

    std::string find_k_lex(int k) {
        int u = 0;
        std::string res;
        while (1) {
            for (item_t elem : data[u].go) {
                int v = elem.second;
                if (data[v].dp >= k) {
                    char c = elem.first;
                    res.push_back(c);
                    u = v;
                    k -= data[u].end;
                    if (k <= 0) {
                        return res;
                    }
                    break;
                } else {
                    k -= data[v].dp;
                }
            }
        }
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, trie_t& tr) {
        out << "root\n";
        tr.print(0, out, 1);
        return out;
    }

    ~trie_t() = default;
};

#endif /* TRIE_HPP */
