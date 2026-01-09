#ifndef FFT
#define FFT

#include <algorithm>
#include <array>
#include <complex>
#include <iostream>
#include <vector>

using fp = double;
using vfp = std::vector<fp>;
using c = std::complex<fp>;
using vc = std::vector<c>;

constexpr int FFT_MAX_K = 22;
int rev_bits(int x) {
    int y = 0;
    for (int i = 0; i < FFT_MAX_K; ++i) {
        y = y ^ (x & 1);
        y = y << 1;
        x = x >> 1;
    }
    return (y >> 1);
}

bool flag_fft_precalc = false;
std::array<int, 1 << FFT_MAX_K> fft_rev;
std::array<c, 2 * (1 << FFT_MAX_K)> fft_roots;
void fft_precalc() {
    for (int i = 0; i < (1 << FFT_MAX_K); ++i) fft_rev[i] = rev_bits(i);
    const fp PI = std::acos(-1);
    for (int cluster = 2; cluster <= (1 << FFT_MAX_K); cluster *= 2) {
        fft_roots[cluster] = c(1, 0);
        c wn(std::cos(2 * PI / cluster), std::sin(2 * PI / cluster));
        for (int i = 1; i < cluster / 2; ++i)
            fft_roots[cluster + i] = fft_roots[cluster + i - 1] * wn;
    }
    flag_fft_precalc = 1;
}

void fft(vc& a, bool inverse = false) {
    if (!flag_fft_precalc) fft_precalc();
    // Look at the Wandermond matrix: you don't need inverse of wn!
    if (inverse) std::reverse(a.begin() + 1, a.end());
    // Count tailing zeros
    int n = a.size(), lg_n = __builtin_ctz(n);
    for (int i = 0; i < n; ++i) {
        int rev_i = fft_rev[i] >> (FFT_MAX_K - lg_n);
        if (i < rev_i) std::swap(a[i], a[rev_i]);
    }
    for (int cluster = 2; cluster <= n; cluster *= 2) {
        for (int j = 0; j < n; j += cluster) {
            for (int k = 0; k < cluster / 2; ++k) {
                c u = a[j + k];
                c v = a[j + k + cluster / 2] * fft_roots[cluster + k];
                a[j + k] = u + v;
                a[j + k + cluster / 2] = u - v;
            }
        }
    }
    if (inverse)
        for (int i = 0; i < n; ++i) a[i] /= n;
}

struct polynom {
    vfp data;

    polynom() : data(1) {}
    polynom(int n) : data(n) {}
    polynom(int n, const fp& el) : data(n, el) {}
    polynom(const vfp& coef) : data(coef) {}

    int size() const { return data.size(); }
    void resize(int n) { data.resize(n); }

    fp& operator[](int id) { return data[id]; }
    const fp& operator[](int id) const { return data[id]; }

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

    polynom& operator*=(fp lambda) {
        for (int i = 0; i < size(); ++i) data[i] *= lambda;
        return *this;
    }

    polynom& operator/=(fp lambda) {
        for (int i = 0; i < size(); ++i) data[i] /= lambda;
        return *this;
    }

    friend polynom operator+(const polynom& lhs, const polynom& rhs) {
        return (polynom(lhs) += rhs);
    }

    friend polynom operator-(const polynom& lhs, const polynom& rhs) {
        return (polynom(lhs) -= rhs);
    }

    friend polynom operator*(fp lambda, const polynom& p) { return (polynom(p) *= lambda); }

    friend polynom operator/(const polynom& p, fp lambda) { return (polynom(p) /= lambda); }

    friend polynom multiply_naive(const polynom& lhs, const polynom& rhs) {
        polynom res(lhs.size() + rhs.size() - 1);
        for (int i = 0; i < lhs.size(); ++i)
            for (int j = 0; j < rhs.size(); ++j) res[i + j] += lhs[i] * rhs[j];
        return res;
    }

    friend polynom multiply_fft(const polynom& lhs, const polynom& rhs) {
        int out_sz = lhs.size() + rhs.size() - 1, lg_sz = 32 - __builtin_clz(out_sz),
            n = 1 << lg_sz;
        vc a(n), b(n);
        for (int i = 0; i < lhs.size(); ++i) a[i].real(lhs[i]);
        for (int i = 0; i < rhs.size(); ++i) b[i].real(rhs[i]);
        fft(a), fft(b);
        for (int i = 0; i < n; ++i) a[i] *= b[i];
        fft(a, true);
        polynom res(out_sz);
        for (int i = 0; i < out_sz; ++i) res[i] = a[i].real();
        return res;
    }

    // Optimize multiplication by low degree polynom
    constexpr static int FFT_NAIVE_SZ_ = 60;

    friend polynom operator*(const polynom& lhs, const polynom& rhs) {
        if (std::min(lhs.size(), rhs.size()) < FFT_NAIVE_SZ_)
            return multiply_naive(lhs, rhs);
        else
            return multiply_fft(lhs, rhs);
    }

    // Use this after each multiplication to do computations modulo mod
    polynom& operator%=(fp mod) {
        for (fp& el : data) {
            fp div = std::round(el / mod);
            el = std::round(el - mod * div);
        }
        return *this;
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
};

#endif /* FFT */
