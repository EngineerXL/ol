#ifndef SUFFIX_AUTOMATON
#define SUFFIX_AUTOMATON

#include <iostream>
#include <map>
#include <string>
#include <vector>

class suffix_automaton_t {
   private:
    struct node_t {
        std::map<char, int> go;
        // Suffix link
        int link = -1;
        // Max length of string ending in current state
        int len = 0;
        // Terminal state
        int end = 0;

        friend std::ostream& operator<<(std::ostream& out, node_t& item) {
            for (item_t elem : item.go) {
                out << "edge = (" << elem.first << ", " << elem.second
                    << "),\n";
            }
            out << "link = " << item.link << ",\n";
            out << "len = " << item.len << '\n';
            return out;
        }
    };

    std::vector<node_t> data;
    int last;

    bool can_go(int u, char c) { return data[u].go.count(c); }

    int& go(int u, char c) { return data[u].go[c]; }

    int& link(int u) { return data[u].link; }

    int& len(int u) { return data[u].len; }

    int& end(int u) { return data[u].end; }

    int size() { return data.size(); }

    int create_node() {
        data.emplace_back();
        return size() - 1;
    }

    int create_clone(int u) {
        data.push_back(data[u]);
        return size() - 1;
    }

    friend std::ostream& operator<<(std::ostream& out, suffix_automaton_t& sa) {
        for (int i = 0; i < sa.size(); ++i) {
            out << "id = " << i << '\n' << sa.data[i] << '\n';
        }
        return out;
    }

   public:
    using item_t = std::pair<char, int>;

    suffix_automaton_t(const std::string& s) : suffix_automaton_t() {
        data.reserve(2 * s.size());
        build(s);
    }

    suffix_automaton_t() : last(0) { create_node(); }

    void mark_term() {
        int p = last;
        while (p) {
            ++end(p);
            p = link(p);
        }
    }

    void build(const std::string& s) {
        for (char c : s) {
            build(c);
        }
        mark_term();
    }

    void build(char c) {
        int cur = create_node();
        len(cur) = len(last) + 1;
        int p = last;
        while (p >= 0 && !can_go(p, c)) {
            go(p, c) = cur;
            p = link(p);
        }
        if (p == -1) {
            link(cur) = 0;
        } else {
            int q = go(p, c);
            if (len(q) == len(p) + 1) {
                link(cur) = q;
            } else {
                int clone = create_clone(q);
                len(clone) = len(p) + 1;
                link(cur) = clone;
                link(q) = clone;
                while (p >= 0 && go(p, c) == q) {
                    go(p, c) = clone;
                    p = link(p);
                }
            }
        }
        last = cur;
    }

    bool find(const std::string& s) {
        int u = 0;
        for (char c : s) {
            if (!can_go(u, c)) {
                return false;
            } else {
                u = go(u, c);
            }
        }
        return true;
    }
};

#endif /* SUFFIX_AUTOMATON */
