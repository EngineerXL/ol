#ifndef DYNAMIC_HULL
#define DYNAMIC HULL

#include <array>
#include <set>

/*
 * Dynamic convex hull, that supports:
 *
 * 1. Point addition
 * 2. Check if point is inside hull
 * 3. Maintains current edges of hull
 *
 * For example you could maintain the area of the hull
 */
template <class T>
struct dynamic_hull_t {
    using point_t = std::array<T, 2>;
    static point_t sub(const point_t& a, const point_t& b) { return {a[0] - b[0], a[1] - b[1]}; }
    static T cross(const point_t& a, const point_t& b) { return a[0] * b[1] - b[0] * a[1]; }
    static point_t rot180(const point_t& a) { return {-a[0], -a[1]}; }

    struct envelope_t {
        std::set<point_t> ps;
        std::set<point_t>::iterator it;

        void rem_point() {
            if (it != ps.begin()) rem_edge(*it, *prev(it));
            if (next(it) != ps.end()) rem_edge(*next(it), *it);
            it = ps.erase(it);
            if (it != ps.end() && it != ps.begin()) add_edge(*it, *prev(it));
        }

        void add_point(const point_t& p) {
            if (it != ps.end() && it != ps.begin()) rem_edge(*it, *prev(it));
            it = ps.insert(p).first;
            if (it != ps.begin()) add_edge(*it, *prev(it));
            if (next(it) != ps.end()) add_edge(*next(it), *it);
        }

        void insert(const point_t& p) {
            if (is_under(p)) return;
            if (ps.size() < 2) {
                add_point(p);
                return;
            }
            if (it != ps.end())
                while (next(it) != ps.end()) {
                    point_t cur = *it, nxt = *next(it);
                    if (cross(sub(nxt, cur), sub(p, cur)) >= 0) {
                        rem_point();
                    } else {
                        break;
                    }
                }
            if (it != ps.begin())
                while (prev(it) != ps.begin()) {
                    it = prev(it);
                    auto cur = *it, prv = *prev(it);
                    if (cross(sub(cur, prv), sub(p, prv)) >= 0) {
                        rem_point();
                    } else {
                        it = next(it);
                        break;
                    }
                }
            add_point(p);
        }

        int is_under(const point_t& p) {
            it = ps.lower_bound(p);
            if (it == ps.end()) return 0;
            // Region is open (set without border) on the left
            // That allows to correctly handle
            // leftmost and rightmost points of the hull
            if (it == ps.begin()) return p == *it ? 2 : 0;
            point_t cur = *it, prv = *prev(it);
            T cr = cross(sub(cur, prv), sub(p, prv));
            if (cr < 0) {
                return 1;
            } else if (cr == 0) {
                return 2;
            } else {
                return 0;
            }
        }

        // Example
        T area = 0;
        void add_edge(const point_t& a, const point_t& b) { area += cross(a, b); }
        void rem_edge(const point_t& a, const point_t& b) { area -= cross(a, b); }
    };

    // Example
    T get() { return upper.area + lower.area; }

    envelope_t upper, lower;

    void insert(const point_t& p) {
        upper.insert(p);
        lower.insert(rot180(p));
    }

    // 0 - false
    // 1 - under
    // 2 - on border
    int is_inside(const point_t& p) {
        int ru = upper.is_under(p), rd = lower.is_under(rot180(p));
        if (ru == 2 || rd == 2) {
            return 2;
        } else {
            return ru && rd;
        }
    }
};

#endif /* DYNAMIC_HULL */
