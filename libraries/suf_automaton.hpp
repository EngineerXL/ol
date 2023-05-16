#ifndef SUFFIX_AUTOMATON
#define SUFFIX_AUTOMATON

#include <map>
#include <vector>

class suffix_automaton_t {
   private:
    struct node_t {
        std::map<char, int> go;
        int link = -1;
        int len = 0;

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
    const size_t MAX_SIZE = 2e6;
    int size;
    int last;

    bool can_go(int u, char c) { return data[u].go.count(c); }

    int go(int u, char c) { return data[u].go[c]; }

    int link(int u) { return data[u].link; }

    int len(int u) { return data[u].len; }

    int create_node() { return ++size; }

    int create_clone(int u) {
        data[++size] = data[u];
        return size;
    }

    friend std::ostream& operator<<(std::ostream& out, suffix_automaton_t& sa) {
        for (int i = 0; i <= sa.size; ++i) {
            out << "id = " << i << '\n' << sa.data[i] << '\n';
        }
        return out;
    }

   public:
    using item_t = std::pair<char, int>;

    suffix_automaton_t(const std::string& s) {
        size = 0;
        last = 0;
        data.resize(2 * s.size());
        build(s);
    }

    suffix_automaton_t() {
        size = 0;
        last = 0;
        data.resize(MAX_SIZE);
    }

    void build(const std::string& s) {
        for (char c : s) {
            build(c);
        }
    }

    void build(char c) {
        int cur = create_node();
        data[cur].len = len(last) + 1;
        int p = last;
        while (p >= 0 and !can_go(p, c)) {
            data[p].go[c] = cur;
            p = link(p);
        }
        if (p == -1) {
            data[cur].link = 0;
        } else {
            int q = go(p, c);
            if (len(q) == len(p) + 1) {
                data[cur].link = q;
            } else {
                int clone = create_clone(q);
                data[clone].len = len(p) + 1;
                data[cur].link = clone;
                data[q].link = clone;
                while (p >= 0 and go(p, c) == q) {
                    data[p].go[c] = clone;
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

    ~suffix_automaton_t() = default;
};

#endif /* SUFFIX_AUTOMATON */
