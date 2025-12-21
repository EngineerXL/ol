#ifndef NTT
#define NTT

#include <algorithm>
#include <array>
#include <cassert>
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

    static int primitive_root() {
        if (mod == 1000000007) return 5;
        if (mod == 998244353) return 3;
        if (mod == 786433) return 10;

        static int root = -1;
        if (root != -1) return root;

        assert(false);
        // todo calc for any modulo
    }
};

// const int MOD = 1e9 + 7;
const int MOD = 998244353;
using mint = modular_int_t<MOD>;
using vm = std::vector<mint>;

constexpr int MAX_K = __builtin_ctz(MOD - 1);
int rev_bits(int x) {
    int y = 0;
    for (int i = 0; i < MAX_K; ++i) {
        y = y ^ (x & 1);
        y = y << 1;
        x = x >> 1;
    }
    return (y >> 1);
}

bool flag_ntt_precalc = false;
std::array<int, 1 << MAX_K> ntt_rev;
std::array<mint, 2 * (1 << MAX_K)> ntt_roots;
void ntt_precalc() {
    for (int i = 0; i < (1 << MAX_K); ++i) ntt_rev[i] = rev_bits(i);
    mint g = mint::primitive_root(), w = g.power((MOD - 1) / (1 << MAX_K));
    for (int cluster = 2; cluster <= (1 << MAX_K); cluster *= 2) {
        ntt_roots[cluster] = 1;
        mint wn = w.power((1 << MAX_K) / cluster);
        for (int i = 1; i < cluster / 2; ++i)
            ntt_roots[cluster + i] = ntt_roots[cluster + i - 1] * wn;
    }
    flag_ntt_precalc = 1;
}

void ntt(vm& a, bool inverse = false) {
    if (!flag_ntt_precalc) ntt_precalc();
    // Look at the Wandermond matrix: you don't need inverse of wn!
    if (inverse) std::reverse(a.begin() + 1, a.end());
    // Count tailing zeros
    int n = a.size(), lg_n = __builtin_ctz(n);
    for (int i = 0; i < n; ++i) {
        int rev_i = ntt_rev[i] >> (MAX_K - lg_n);
        if (i < rev_i) std::swap(a[i], a[rev_i]);
    }
    for (int cluster = 2; cluster <= n; cluster *= 2) {
        for (int j = 0; j < n; j += cluster) {
            for (int k = 0; k < cluster / 2; ++k) {
                mint u = a[j + k];
                mint v = a[j + k + cluster / 2] * ntt_roots[cluster + k];
                a[j + k] = u + v;
                a[j + k + cluster / 2] = u - v;
            }
        }
    }
    if (inverse) {
        mint rev_n = mint(n).inverse();
        for (int i = 0; i < n; ++i) a[i] *= rev_n;
    }
}

struct polynom {
    vm data;

    polynom() : data(1) {}
    polynom(int n) : data(n) {}
    polynom(int n, const mint& el) : data(n, el) {}
    polynom(const vm& coef) : data(coef) {}

    int size() const { return data.size(); }
    void resize(int n) { data.resize(n); }

    mint& operator[](int id) { return data[id]; }
    const mint& operator[](int id) const { return data[id]; }

    polynom& operator+=(const polynom& rhs) {
        if (rhs.size() > size()) resize(rhs.size());
        for (int i = 0; i < rhs.size(); ++i) data[i] += rhs[i];
        return *this;
    }

    polynom& operator-=(const polynom& rhs) {
        if (rhs.size() > size()) resize(rhs.size());
        for (int i = 0; i < rhs.size(); ++i) data[i] -= rhs[i];
        return *this;
    }

    polynom& operator*=(mint lambda) {
        for (int i = 0; i < size(); ++i) data[i] *= lambda;
        return *this;
    }

    polynom& operator/=(mint lambda) { return *this *= lambda.inverse(); }

    friend polynom operator+(const polynom& lhs, const polynom& rhs) {
        return (polynom(lhs) += rhs);
    }

    friend polynom operator-(const polynom& lhs, const polynom& rhs) {
        return (polynom(lhs) -= rhs);
    }

    friend polynom operator*(mint lambda, const polynom& p) { return (polynom(p) *= lambda); }

    friend polynom operator/(const polynom& p, mint lambda) { return (polynom(p) /= lambda); }

    friend polynom multiply_naive(const polynom& lhs, const polynom& rhs) {
        polynom res(lhs.size() + rhs.size() - 1);
        for (int i = 0; i < lhs.size(); ++i)
            for (int j = 0; j < rhs.size(); ++j) res[i + j] += lhs[i] * rhs[j];
        return res;
    }

    friend polynom multiply_ntt(const polynom& lhs, const polynom& rhs) {
        int out_sz = lhs.size() + rhs.size() - 1, lg_sz = 32 - __builtin_clz(out_sz),
            n = 1 << lg_sz;
        vm a(lhs.data), b(rhs.data);
        a.resize(n), b.resize(n);
        ntt(a), ntt(b);
        for (int i = 0; i < n; ++i) a[i] *= b[i];
        ntt(a, true);
        // Shrinks result for better performance in D&C tasks
        a.resize(out_sz);
        return a;
    }

    // Optimize multiplication by low degree polynom
    constexpr static int FFT_NAIVE_SZ_ = 60;

    friend polynom operator*(const polynom& lhs, const polynom& rhs) {
        if (std::min(lhs.size(), rhs.size()) < FFT_NAIVE_SZ_)
            return multiply_naive(lhs, rhs);
        else
            return multiply_ntt(lhs, rhs);
    }

    friend std::istream& operator>>(std::istream& in, polynom& poly) {
        int deg;
        in >> deg;
        int n = deg + 1;
        poly.resize(n);
        for (int i = 0; i < n; ++i) in >> poly[i];
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const polynom& poly) {
        for (int i = 0; i < poly.size(); ++i) {
            if (i) out << ' ';
            out << poly[i];
        }
        return out;
    }

    /*
     * Computes p ^ y mod x ^ m, result has size m
     * Complexity: O(m log m log y)
     */
    template <class T>
    polynom modpow(T y, int m) const {
        static_assert(std::is_integral<T>::value);
        polynom res(1, 1), p(data);
        while (y > 0) {
            if (y % 2) {
                res = res * p;
                if (res.size() > m) res.resize(m);
            }
            p = p * p;
            if (p.size() > m) p.resize(m);
            y /= 2;
        }
        res.resize(m);
        return res;
    }

    /*
     * For a given polynomial f(x) computes polynomial g(x)
     * such that f(x) * g(x) = 1 mod x^m
     * Complexity: O(m log m)
     */
    polynom inverse(int m) {
        polynom p(data);
        int lg_m = 32 - __builtin_clz(m);
        p.resize(1 << lg_m);
        polynom q(1, p[0].inverse());
        // MW Pre-Finals 2022 FFT Lecture
        for (int step = 0; step < lg_m; ++step) {
            int cur_m = 1 << step;
            polynom p0(cur_m), p1(cur_m);
            for (int i = 0; i < cur_m; ++i) {
                p0[i] = p[i];
                p1[i] = p[i + cur_m];
            }
            polynom rem = q * p0, r(cur_m);
            rem.resize(2 * cur_m);
            for (int i = 0; i < cur_m; ++i) r[i] = rem[i + cur_m];
            polynom t = q * (r + (p1 * q)), qs(cur_m * 2);
            for (int i = 0; i < cur_m; ++i) {
                qs[i] = q[i];
                qs[i + cur_m] = -t[i];
            }
            q = qs;
        }
        q.resize(m);
        return q;
    }
};

#endif /* NTT */
