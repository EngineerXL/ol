#ifndef STRINGS
#define STRINGS

#include <vector>

template <class T>
std::vector<int> z_func(const T& s) {
    int n = s.size();
    std::vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i < r) z[i] = std::min(z[i - l], r - i);
        while (i + z[i] < n && s[i + z[i]] == s[z[i]]) ++z[i];
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}

template <class T>
std::vector<int> p_func(const T& s) {
    int n = s.size();
    std::vector<int> p(n);
    for (int i = 1; i < n; ++i) {
        p[i] = p[i - 1];
        while (p[i] > 0 && s[i] != s[p[i]]) p[i] = p[p[i] - 1];
        p[i] += s[i] == s[p[i]];
    }
    return p;
}

#endif /* STRINGS */
