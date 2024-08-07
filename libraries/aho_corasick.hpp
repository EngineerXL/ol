#ifndef AHO_CORASIKH
#define AHO_CORASIKH

#include <algorithm>
#include <map>
#include <string>
#include <vector>

struct trie_t {
    struct node_t {
        std::map<char, int> go;
        int end = 0;
        int dp = 0;
        std::vector<int> ids;
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

    void insert(const std::string& s, int id = 0) {
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
        data[u].ids.push_back(id);
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

struct aho_corasikh : trie_t {
    aho_corasikh() = default;

    aho_corasikh(const std::vector<std::string>& s) {
        for (size_t i = 0; i < s.size(); ++i) {
            insert(s[i], i);
        }
        build();
    }

    const int S = 26;
    const char A = 'a';

    int n;
    std::vector<int> suff, word, q;
    std::vector<item_t> parent;
    std::vector<std::vector<int>> atmt, g;
    void build() {
        n = data.size();
        suff.resize(n), word.resize(n);
        parent.resize(n, {'$', -1});
        atmt.resize(n, std::vector<int>(S)), g.resize(n);
        for (int u = 0; u < n; ++u) {
            for (auto [c, v] : data[u].go) {
                parent[v] = {c, u};
            }
        }
        q.reserve(n);
        size_t ptr = 0;
        for (q.push_back(0); ptr < q.size(); ++ptr) {
            int u = q[ptr];
            for (auto [_, v] : data[u].go) {
                q.push_back(v);
            }
            if (u) {
                // suff
                auto [c, v] = parent[u];
                v = suff[v];
                while (v && !can_go(v, c)) {
                    v = suff[v];
                }
                if (can_go(v, c) && go(v, c) != u) {
                    suff[u] = go(v, c);
                }
                g[suff[u]].push_back(u);
                // word
                v = suff[u];
                word[u] = end(v) ? v : word[v];
            }
            // atmt
            for (int j = 0; j < S; ++j) {
                if (can_go(u, A + j)) {
                    atmt[u][j] = go(u, A + j);
                } else {
                    atmt[u][j] = atmt[suff[u]][j];
                }
            }
        }
        std::reverse(q.begin(), q.end());
    }

    using vl = std::vector<long long>;
    vl find_all_cnt(const std::string& s) {
        vl vis(n, 0);
        int u = 0;
        for (char c : s) {
            u = atmt[u][c - A];
            ++vis[u];
        }
        for (int u : q) {
            for (int v : g[u]) {
                vis[u] += vis[v];
            }
        }
        vl res(data[0].dp);
        for (int u = 0; u < n; ++u) {
            for (int elem : data[u].ids) {
                res[elem] = vis[u];
            }
        }
        return res;
    }

    using vi = std::vector<int>;
    std::vector<vi> find_all_occ(const std::string& s) {
        std::vector<vi> res(data[0].dp);
        int u = 0, k = s.size();
        for (int i = 0; i < k; ++i) {
            char c = s[i];
            u = atmt[u][c - A];
            int v = end(u) ? u : word[u];
            while (v) {
                for (int el : data[v].ids) {
                    res[el].push_back(i);
                }
                v = word[v];
            }
        }
        return res;
    }
};

#endif /* AHO_CORASIKH */
