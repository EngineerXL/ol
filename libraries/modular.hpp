#ifndef MODULAR
#define MODULAR

#include <iostream>

template <int64_t mod>
struct modular_t {
    int64_t value;

    modular_t() : value() {}

    template <class T>
    modular_t(T x) {
        value = normalize(x);
    }

    template <class T>
    static int64_t normalize(T x) {
        int64_t val;
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
        int64_t val;
        in >> val;
        num.value = normalize(val);
        return in;
    }

    template <class T>
    friend modular_t power(modular_t a, T deg) {
        int64_t x = a.value;
        int64_t y = deg;
        int64_t z = 1;
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
