#ifndef COMBINATORICS
#define COMBINATORICS

#include <iostream>
#include <limits>
#include <vector>

/*
 * Modular integer class
 * MOD must be prime to use inverse and division
 * 2 * (MOD - 1) <= INT_MAX must hold
 */
template <int mod>
struct modular_int_t {
    static_assert(mod - 2 <= std::numeric_limits<int>::max() - mod, "2 * (mod - 1) <= INT_MAX");
    using mint = modular_int_t<mod>;

    int value;

    modular_int_t() : value(0) {}
    modular_int_t(const mint& other) : value(other.value) {}

    template <class T>
    modular_int_t(T x) : value(normalize(x)) {}

    template <class T>
    static int normalize(T x) {
        static_assert(std::is_integral<T>::value);
        if (x < -mod || x > 2 * mod - 1) x %= mod;
        if (x >= mod) x -= mod;
        if (x < 0) x += mod;
        return x;
    }

    template <class T>
    mint power(T deg) const {
        static_assert(std::is_integral<T>::value);
        mint res(1), x = *this;
        while (deg > 0) {
            if (deg & 1) res *= x;
            x *= x;
            deg >>= 1;
        }
        return res;
    }

    mint inverse() const { return power(mod - 2); }

    mint& operator=(const mint& other) {
        value = other.value;
        return *this;
    }

    mint& operator+=(const mint& other) {
        if ((value += other.value) >= mod) value -= mod;
        return *this;
    }

    mint& operator-=(const mint& other) {
        if ((value -= other.value) < 0) value += mod;
        return *this;
    }

    mint& operator*=(const mint& other) {
        value = (int64_t(value) * other.value) % mod;
        return *this;
    }

    mint& operator/=(const mint& other) { return *this *= other.inverse(); }

    friend mint operator+(const mint& lhs, const mint& rhs) { return (mint(lhs) += rhs); }

    friend mint operator-(const mint& lhs, const mint& rhs) { return (mint(lhs) -= rhs); }

    friend mint operator*(const mint& lhs, const mint& rhs) { return (mint(lhs) *= rhs); }

    friend mint operator/(const mint& lhs, const mint& rhs) { return (mint(lhs) /= rhs); }

    mint operator-() { return mint(-value); }

    friend bool operator==(const mint& lhs, const mint& rhs) { return lhs.value == rhs.value; }

    friend bool operator!=(const mint& lhs, const mint& rhs) { return lhs.value != rhs.value; }

    friend bool operator<(const mint& lhs, const mint& rhs) { return lhs.value < rhs.value; }

    friend bool operator>(const mint& lhs, const mint& rhs) { return lhs.value > rhs.value; }

    mint& operator++() {
        ++value;
        if (value == mod) value = 0;
        return *this;
    }

    mint& operator--() {
        --value;
        if (value == -1) value = mod - 1;
        return *this;
    }

    mint operator++(int) {
        mint prev = *this;
        value++;
        if (value == mod) value = 0;
        return prev;
    }

    mint operator--(int) {
        mint prev = *this;
        value--;
        if (value == -1) value = mod - 1;
        return prev;
    }

    friend std::istream& operator>>(std::istream& in, mint& num) {
        int64_t val;
        in >> val;
        num.value = normalize(val);
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const mint& num) {
        out << num.value;
        return out;
    }
};

// const int MOD = 1e9 + 7;
const int MOD = 998244353;
using mint = modular_int_t<MOD>;
using vm = std::vector<mint>;

// No need to run any init function, it dynamically resizes the data.
namespace combinatorics {

vm fact_, inv_fact_, inv_;

void resize_data(int n) {
    if (fact_.empty()) {
        fact_ = {1, 1};
        inv_fact_ = {1, 1};
        inv_ = {0, 1};
    }
    for (int cur = fact_.size(); cur <= n; ++cur) {
        fact_.push_back(fact_.back() * cur);
        inv_.push_back(mint(cur).inverse());
        inv_fact_.push_back(inv_fact_.back() * inv_.back());
    }
}

struct comb_data {
    vm& data;

    mint operator[](int i) {
        if (i >= static_cast<int>(data.size())) resize_data(i);
        return data[i];
    }
} fact(fact_), inv_fact(inv_fact_), inv(inv_);

/*
 * From n choose k
 * (n, k) = (n - 1, k - 1) + (n, k - 1)
 */
mint choose(int n, int k) {
    if (n < k || n < 0 || k < 0) return 0;
    return fact[n] * inv_fact[k] * inv_fact[n - k];
}

// Number of balanced bracket sequences with n open and k closing brackets.
mint catalan(int n, int k) {
    if (k > n || k < 0) return 0;
    return choose(n + k, k) - choose(n + k, k - 1);
}

/*
 * C_n = 1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, ...
 * - sub-diagonal monotone paths in an n × n grid.
 * - strings with n pairs of parenthesis, correctly nested.
 * - binary trees with with n + 1 leaves(0 or 2 children).
 * - ordered trees with n + 1 vertices.
 * - ways a convex polygon with n + 2 sides can be cut into triangles
 * by connecting vertices with straight lines.
 * - permutations of [n] with no 3-term increasing subseq.
 */
mint catalan(int n) { return catalan(n, n); }

/*
 * Stirling numbers of the second kind
 * Partitions of n distinct elements into exactly k groups.
 * S(n, k) = S(n - 1, k - 1) + kS(n - 1, k)
 * S(n, 1) = S(n, n) = 1
 */
mint stirling2(int n, int k) {
    mint res = 0;
    for (int j = k, sgn = 1; j >= 0; --j, sgn *= -1) res += sgn * choose(k, j) * mint(j).power(n);
    return res * inv_fact[k];
}

};  // namespace combinatorics

using namespace combinatorics;

#endif /* COMBINATORICS */
