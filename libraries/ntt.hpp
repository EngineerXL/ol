#ifndef MODULAR
#define MODULAR

#include <iostream>

using ll = signed long long int;

template <ll mod>
struct modular_t {
    ll value;

    modular_t() : value() {}

    template <class T>
    modular_t(T x) {
        value = normalize(x);
    }

    template <class T>
    static ll normalize(T x) {
        ll val;
        if (-mod <= x and x < mod) {
            val = x;
        } else {
            val = x % mod;
        }
        return (val < 0 ? val + mod : val);
    }

    modular_t operator-() { return modular_t(-value); }

    modular_t& operator+=(const modular_t& other) {
        if ((value += other.value) >= mod) {
            value -= mod;
        }
        return *this;
    }

    modular_t& operator-=(const modular_t& other) {
        if ((value -= other.value) < 0) {
            value += mod;
        }
        return *this;
    }

    modular_t& operator*=(const modular_t& other) {
        value = normalize(value * other.value);
        return *this;
    }

    friend modular_t operator+(const modular_t& lhs, const modular_t& rhs) {
        return (modular_t(lhs) += rhs);
    }

    template <class T>
    friend modular_t operator+(const modular_t& lhs, const T& rhs) {
        return (modular_t(lhs) += modular_t(rhs));
    }

    template <class T>
    friend modular_t operator+(const T& lhs, const modular_t& rhs) {
        return (modular_t(lhs) += rhs);
    }

    friend modular_t operator-(const modular_t& lhs, const modular_t& rhs) {
        return (modular_t(lhs) -= rhs);
    }

    template <class T>
    friend modular_t operator-(const modular_t& lhs, const T& rhs) {
        return (modular_t(lhs) -= modular_t(rhs));
    }

    template <class T>
    friend modular_t operator-(const T& lhs, const modular_t& rhs) {
        return (modular_t(lhs) -= rhs);
    }

    friend modular_t operator*(const modular_t& lhs, const modular_t& rhs) {
        return (modular_t(lhs) *= rhs);
    }

    template <class T>
    friend modular_t operator*(const modular_t& lhs, const T& rhs) {
        return (modular_t(lhs) *= modular_t(rhs));
    }

    template <class T>
    friend modular_t operator*(const T& lhs, const modular_t& rhs) {
        return (modular_t(lhs) *= rhs);
    }

    friend bool operator<(const modular_t& lhs, const modular_t& rhs) {
        return lhs.value < rhs.value;
    }

    friend bool operator>(const modular_t& lhs, const modular_t& rhs) {
        return lhs.value > rhs.value;
    }

    friend bool operator==(const modular_t& lhs, const modular_t& rhs) {
        return lhs.value == rhs.value;
    }

    friend bool operator!=(const modular_t& lhs, const modular_t& rhs) {
        return lhs.value != rhs.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const modular_t& num) {
        out << num.value;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, modular_t& num) {
        ll val;
        in >> val;
        num.value = normalize(val);
        return in;
    }

    template <class T>
    friend modular_t power(modular_t a, T deg) {
        ll x = a.value;
        ll y = deg;
        ll z = 1;
        while (y) {
            if (y & 1) {
                z = (z * x) % mod;
            }
            x = (x * x) % mod;
            y = y >> 1;
        }
        return modular_t(z);
    }

    modular_t inverse() { return power(*this, mod - 2); }
};

#endif /* MODULAR */

#ifndef NTT
#define NTT

#include <complex>
#include <iostream>
#include <vector>

// const int K = 20;
// const ll MOD = 7340033;
// const ll WN = 5;
const int K = 23;
const ll MOD = (119 << 23) + 1;
const ll WN = 31;

using base = modular_t<MOD>;
using vc = std::vector<base>;

const ll WN_INVERSE = base(WN).inverse().value;

int lg_2(ll x) {
    int y = 0;
    while ((1 << y) < x) {
        ++y;
    }
    return y;
}

ll rev_bits(ll x, int n) {
    ll y = 0;
    for (int i = 0; i < n; ++i) {
        y = y ^ (x & 1);
        y = y << 1;
        x = x >> 1;
    }
    return (y >> 1);
}

void ntt(vc& a, bool inverse = false) {
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
        base wn(inverse ? WN_INVERSE : WN);
        for (int i = cluster; i < (1 << K); i <<= 1) {
            wn = wn * wn;
        }
        for (int j = 0; j < n; j += cluster) {
            base w(1);
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
        base rev_n = base(n).inverse();
        for (int i = 0; i < n; ++i) {
            a[i] *= rev_n;
        }
    }
}

struct polynom {
    using vec = std::vector<base>;

    vec data;
    size_t n;

    polynom() : data(1), n(1) {}

    polynom(int _n) : data(_n), n(_n) {}

    polynom(int _n, const base& el) : data(_n, el), n(_n) {}

    polynom(const vec& coef) : data(coef), n(data.size()) {}

    size_t size() const { return n; }

    void resize(size_t _n) {
        n = _n;
        data.resize(n);
    }

    base& operator[](size_t id) { return data[id]; }

    const base& operator[](size_t id) const { return data[id]; }

    friend polynom operator+(const polynom& lhs, const polynom& rhs) {
        polynom res(std::max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < lhs.size(); ++i) {
            res[i] += lhs[i];
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            res[i] += rhs[i];
        }
        return res;
    }

    friend polynom operator-(const polynom& lhs, const polynom& rhs) {
        polynom res(std::max(lhs.size(), rhs.size()));
        for (size_t i = 0; i < lhs.size(); ++i) {
            res[i] += lhs[i];
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            res[i] -= rhs[i];
        }
        return res;
    }

    friend polynom operator*(base lambda, const polynom& p) {
        polynom res(p);
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] *= lambda;
        }
        return res;
    }

    friend polynom operator/(const polynom& p, base lambda) {
        polynom res(p);
        base rev_lambda(lambda);
        rev_lambda = rev_lambda.inverse();
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] *= rev_lambda;
        }
        return res;
    }

    friend polynom operator*(const polynom& lhs, const polynom& rhs) {
        size_t lr_sz = std::max(lhs.size(), rhs.size());
        int lg_sz = lg_2(lr_sz) + 1;
        size_t n = (1 << lg_sz);
        vc a(n), b(n);
        for (size_t i = 0; i < lhs.size(); ++i) {
            a[i] = base(lhs[i]);
        }
        for (size_t i = 0; i < rhs.size(); ++i) {
            b[i] = base(rhs[i]);
        }
        ntt(a, false);
        ntt(b, false);
        for (size_t i = 0; i < n; ++i) {
            a[i] *= b[i];
        }
        ntt(a, true);
        // Shrinks result for better performance
        // in D&C tasks
        a.resize(lhs.size() + rhs.size() - 1);
        return a;
    }

    friend std::istream& operator>>(std::istream& in, polynom& poly) {
        size_t deg;
        in >> deg;
        size_t n = deg + 1;
        poly.resize(n);
        for (size_t i = 0; i < n; ++i) {
            in >> poly[i];
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const polynom& poly) {
        for (size_t i = 0; i < poly.size(); ++i) {
            if (i) {
                out << ' ';
            }
            out << poly[i];
        }
        return out;
    }

    friend polynom inverse(polynom p, int m) {
        int lg_m = lg_2(m);
        p.resize(1 << lg_m);
        vec q0 = {p[0].inverse()};
        polynom q(q0);
        for (int step = 0; step < lg_m; ++step) {
            int cur_m = 1 << step;
            polynom p0(cur_m);
            polynom p1(cur_m);
            for (int i = 0; i < cur_m; ++i) {
                p0[i] = p[i];
                p1[i] = p[i + cur_m];
            }
            polynom rem = q * p0;
            polynom r(cur_m);
            rem.resize(2 * cur_m);
            for (int i = 0; i < cur_m; ++i) {
                r[i] = rem[i + cur_m];
            }
            polynom t = q * (r + (p1 * q));
            polynom qs(cur_m * 2);
            for (int i = 0; i < cur_m; ++i) {
                qs[i] = q[i];
                qs[i + cur_m] = -1 * t[i];
            }
            q = qs;
        }
        q.resize(m);
        return q;
    }

    ~polynom() = default;
};

#endif /* NTT */
