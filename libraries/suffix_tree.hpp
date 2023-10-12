#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP

#include <iostream>
#include <map>
#include <vector>

template <class T>
class suffix_tree_t {
   private:
    const static int INF = 1e9;
    const static int TAB_SIZE = 4;

    struct node_t {
        std::map<T, int> go;
        int l, len;
        int link = 0;

        node_t(int _l, int _len) : l(_l), len(_len) {}
    };

    using item_t = std::pair<T, int>;

    bool can_go(int u, T c) { return data[u].go.count(c); }

    int go(int u, T c) { return data[u].go[c]; }

    std::vector<T> s;
    std::vector<node_t> data;

    int size;
    int nodes;

    int u = 0;
    int rem = 0;

    void go_edge() {
        do {
            T c = s[size - rem];
            if (!can_go(u, c)) {
                return;
            }
            int v = go(u, c);
            if (rem > data[v].len) {
                rem -= data[v].len;
                u = v;
            } else {
                return;
            }
        } while (1);
    }

    int create_node(int l = 0, int len = INF) {
        data.push_back(node_t(l, len));
        return ++nodes;
    }

    void init_root() {
        create_node(0, INF);
        size = 0;
        nodes = 0;
    }

    void build() {
        for (size_t i = 0; i < s.size(); ++i) {
            extend(s[i]);
        }
    }

    /* Main function */
    void extend(const T& c) {
        ++size;
        ++rem;
        int last = 0;
        while (rem) {
            go_edge();
            T a = s[size - rem];
            if (!can_go(u, a)) {
                /* Case 1: creating a leaf */
                data[u].go[a] = create_node(size - rem);
                data[last].link = u;
                last = u;
                ++leaves;
            } else {
                int v = go(u, a);
                int good_len = rem - 1;
                T t = s[data[v].l + good_len];
                if (t != c) {
                    /* Case 2: splitting an edge */
                    int split = create_node(data[v].l, good_len);
                    int new_v = create_node(size - 1, INF);
                    data[v].len -= good_len;
                    data[v].l += good_len;
                    data[u].go[a] = split;
                    data[split].go[t] = v;
                    data[split].go[c] = new_v;
                    data[last].link = split;
                    last = split;
                    ++leaves;
                    int64_t new_len = edge_len(new_v);
                    substrings += new_len - 1;
                } else {
                    /* Case 3: end of phase */
                    data[last].link = u;
                    substrings += leaves;
                    return;
                }
            }
            if (u == 0) {
                --rem;
            } else {
                u = data[u].link;
            }
        }
        substrings += leaves;
    }

    void print_edge(int v, std::ostream& out) {
        int len = edge_len(v);
        for (int i = 0; i < len; ++i) {
            out << s[data[v].l + i];
        }
    }

    void print(int id, std::ostream& out, int h) {
        for (item_t elem : data[id].go) {
            for (int i = 0; i < TAB_SIZE * (h - 1); ++i) {
                out << ' ';
            }
            int v = elem.second;
            out << "|-> {id = " << v << ", ";
            print_edge(v, out);
            out << ", link = " << data[v].link << "}\n";
            print(v, out, h + 1);
        }
    }

   public:
    suffix_tree_t(const std::vector<T>& _s) : s(_s) {
        init_root();
        build();
    }

    suffix_tree_t() { init_root(); }

    void push_back(const T& c) {
        s.emplace_back(c);
        extend(c);
    }

    friend std::ostream& operator<<(std::ostream& out, suffix_tree_t& st) {
        out << "root\n";
        st.print(0, out, 1);
        return out;
    }

    /* Different substrings */
    int64_t leaves = 0;
    int64_t substrings = 0;

    int edge_len(int u) { return std::min(size - data[u].l, data[u].len); }

    int64_t count_subs() { return substrings; }

    std::vector<T> lex_min(int k) {
        int v = data[0].go.begin()->second;
        int pos = 0;
        std::vector<T> res;
        while (k) {
            if (pos == data[v].len) {
                v = data[v].go.begin()->second;
                pos = 0;
            } else {
                res.emplace_back(s[data[v].l + pos]);
                ++pos;
                --k;
            }
        }
        return res;
    }

    /* Matching statistics */
    bool can_walk(const std::vector<T>& t, int id, int from, int to, int c_id,
                  int pos) {
        T c = t[id];
        if (pos == 0) {
            return can_go(from, c);
        } else {
            return s[data[to].l + pos] == c;
        }
    }

    void walk(const std::vector<T>& t, int id, int& from, int& to, int& c_id,
              int& pos) {
        T c = t[id];
        if (pos == 0) {
            to = go(from, c);
            c_id = id;
            pos = 1;
        } else {
            ++pos;
        }
        if (pos == data[to].len) {
            from = to;
            to = -1;
            c_id = -1;
            pos = 0;
        }
    }

    std::vector<int> matching_statistics(const std::vector<T>& t) {
        int n = t.size();
        std::vector<int> res(n);
        int from = 0;
        int to = -1;
        int c_id = -1;
        int pos = 0;
        int r = 0;
        for (int i = 0; i < n; ++i) {
            r = std::max(i, r);
            while (r < n and can_walk(t, r, from, to, c_id, pos)) {
                walk(t, r, from, to, c_id, pos);
                ++r;
            }
            res[i] = r - i;
            if (from == 0) {
                pos = std::max(0, pos - 1);
            } else {
                from = data[from].link;
                to = -1;
                while (pos) {
                    T c = t[c_id];
                    to = go(from, c);
                    if (pos >= data[to].len) {
                        c_id += data[to].len;
                        pos -= data[to].len;
                        from = to;
                        to = -1;
                    } else {
                        break;
                    }
                }
            }
        }
        return res;
    }
};

#endif /* SUFFIX_TREE_HPP */
