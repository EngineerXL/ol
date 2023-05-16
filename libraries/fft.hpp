#ifndef FFT
#define FFT

#include <complex>
#include <iostream>
#include <vector>

const double EPS = 1e-9;
const double PI = acos(-1);

using base = std::complex<double>;

using vc = std::vector<base>;

int lg_2(int64_t x) {
    int y = 0;
    while ((1 << y) < x) {
        ++y;
    }
    return y;
}

int64_t rev_bits(int64_t x, int n) {
    int64_t y = 0;
    for (int i = 0; i < n; ++i) {
        y = y ^ (x & 1);
        y = y << 1;
        x = x >> 1;
    }
    return (y >> 1);
}

void fft(vc &a, bool inverse = false) {
    int n = a.size();
    int lg_n = lg_2(n);
    for (int i = 0; i < n; ++i) {
        int r_i = rev_bits(i, lg_n);
        if (i < r_i) {
            std::swap(a[i], a[r_i]);
        }
    }
    for (int layer = 1; layer <= lg_n; ++layer) {
        int cluster = 1 << layer;
        double phi = (2 * PI) / cluster;
        if (inverse) {
            phi *= -1;
        }
        base wn(cos(phi), sin(phi));
        for (int j = 0; j < n; j += cluster) {
            base w(1, 0);
            for (int k = 0; k < cluster / 2; ++k) {
                base u = a[j + k];
                base v = a[j + k + cluster / 2] * w;
                a[j + k] = u + v;
                a[j + k + cluster / 2] = u - v;
                w *= wn;
            }
        }
    }
    if (inverse) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

struct polynom {
    using vec = std::vector<double>;

    vec data;
    size_t n;

    polynom() : data(1), n(1) {}

    polynom(int _n) : data(_n), n(_n) {}

    polynom(const vec &coef) : data(coef), n(data.size()) {}

    size_t size() const { return n; }

    void resize(size_t _n) {
        n = _n;
        data.resize(n);
    }

    double &operator[](size_t id) { return data[id]; }

    const double &operator[](size_t id) const { return data[id]; }

    friend polynom operator+(const polynom &lhs, const polynom &rhs) {
        polynom res(std::max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < lhs.size(); ++i) {
            res[i] += lhs[i];
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            res[i] += rhs[i];
        }
        return res;
    }

    friend polynom operator-(const polynom &lhs, const polynom &rhs) {
        polynom res(std::max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < lhs.size(); ++i) {
            res[i] += lhs[i];
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            res[i] -= rhs[i];
        }
        return res;
    }

    friend polynom operator*(double lambda, const polynom &p) {
        polynom res(p);
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] *= lambda;
        }
        return res;
    }

    friend polynom operator/(const polynom &p, double lambda) {
        polynom res(p);
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] /= lambda;
        }
        return res;
    }

    friend polynom slow_mul(const polynom &lhs, const polynom &rhs) {
        polynom res(lhs.size() + rhs.size() - 1);
        for (size_t i = 0; i < lhs.size(); ++i) {
            for (size_t j = 0; j < rhs.size(); ++j) {
                res[i + j] += lhs[i] * rhs[j];
            }
        }
        return res;
    }

    friend polynom fast_mul(const polynom &lhs, const polynom &rhs) {
        size_t n = std::max(lhs.size(), rhs.size()) * 2;
        size_t m = (1 << lg_2(n));
        vc a(m), b(m);
        for (size_t i = 0; i < lhs.size(); ++i) {
            a[i] = base(lhs[i]);
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            b[i] = base(rhs[i]);
        }
        fft(a, false);
        fft(b, false);
        for (size_t i = 0; i < m; ++i) {
            a[i] *= b[i];
        }
        fft(a, true);
        polynom res(lhs.size() + rhs.size() - 1);
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] = a[i].real();
        }
        return res;
    }

    constexpr static size_t FFT_N = 1e4;

    friend polynom operator*(const polynom &lhs, const polynom &rhs) {
        if (lhs.size() * rhs.size() < FFT_N) {
            return slow_mul(lhs, rhs);
        } else {
            return fast_mul(lhs, rhs);
        }
    }

    friend std::istream &operator>>(std::istream &in, polynom &poly) {
        size_t deg;
        in >> deg;
        size_t n = deg + 1;
        poly.n = n;
        poly.data.resize(n);
        for (size_t i = 0; i < n; ++i) {
            in >> poly[deg - i];
        }
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const polynom &poly) {
        size_t n = poly.size();
        out << n - 1;
        for (size_t i = 0; i < n; ++i) {
            // out << ' ' << poly[n - 1 - i];
            out << ' ' << (int64_t)(round(poly[n - 1 - i]));
        }
        return out;
    }

    ~polynom() = default;
};

#endif /* FFT */
