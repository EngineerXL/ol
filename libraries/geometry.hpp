#ifndef GEOMETRY
#define GEOMETRY

#include <cmath>
#include <iostream>

template <class T>
struct vector2d_t {
    T x, y;

    using vec2d = vector2d_t<T>;

    vector2d_t() : x(), y(){};

    vector2d_t(const T& _x, const T& _y) : x(_x), y(_y){};

    vector2d_t(const vec2d& v) : x(v.x), y(v.y){};

    vec2d& operator=(const vec2d& other) {
        if (&other == this) {
            return *this;
        }
        x = other.x;
        y = other.y;
        return *this;
    }

    friend vec2d operator+(const vec2d& a, const vec2d& b) {
        return vec2d(a.x + b.x, a.y + b.y);
    }

    vec2d& operator+=(const vec2d& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    friend vec2d operator-(const vec2d& a, const vec2d& b) {
        return vec2d(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    vec2d& operator-=(const vec2d& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    friend vec2d operator*(const T& lambda, const vec2d& v) {
        return {lambda * v.x, lambda * v.y};
    }

    vec2d& operator*=(const T& lambda) {
        x *= lambda;
        y *= lambda;
        return *this;
    }

    friend vec2d operator/(const vec2d& v, const T& lambda) {
        return {v.x / lambda, v.y / lambda};
    }

    vec2d& operator/=(const T& lambda) {
        x /= lambda;
        y /= lambda;
        return *this;
    }

    friend std::istream& operator>>(std::istream& in, vec2d& v) {
        in >> v.x >> v.y;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const vec2d& v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }

    static double dot(const vec2d& a, const vec2d& b) {
        return a.x * b.x + a.y * b.y;
    }

    double len() const { return std::sqrt(dot(*this, *this)); }

    friend T cross(const vec2d& a, const vec2d& b) {
        return a.x * b.y - a.y * b.x;
    }
};

/* Basic types */
using vec2f = vector2d_t<float>;
using vec2d = vector2d_t<double>;
using vec2i = vector2d_t<int>;

#endif /* GEOMETRY */