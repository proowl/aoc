
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip>
#include <map>
#include <set>
#include <functional>
#include <cctype>
#include <limits>
#include <tuple>
#include <deque>
#include <stack>
#include <numeric>
#include <cmath>

using namespace std;

using point2d = array<long, 2>;
//                   dx       dy       dz       # of lit
using area_t = tuple<point2d, point2d, point2d, long>;

vector<tuple<string, point2d, point2d, point2d>> input = {
#ifdef REAL_INPUT
#include "input.h"
#else
#ifdef TEST_LIMITED
    { "on", {10, 12}, {10, 12}, {10, 12}},
    { "on", {11, 13},{11, 13},{11, 13}},
    { "off", {9, 11},{9, 11},{9, 11}},
    { "on", {10, 10},{10, 10},{10, 10}},
#else
#include "test2-input.h"
#endif
#endif
};

ostream& operator<<(ostream& os, point2d const& r) {
    os << setw(5) << r[0] << ".." << r[1];
    return os;
}

ostream& operator<<(ostream& os, area_t const& r) {
    auto [dx,dy,dz,s] = r;
    os << "{x=" << dx << ",y=" << dy << ",z=" << dz << "," << s << "}";
    return os;
}

ostream& operator<<(ostream& os, vector<area_t> const& r) {
    for (auto v : r) {
        os << v << "\n";
    }
    return os;
}

bool is_within(long val, point2d range) {
    return val >= range[0] && val <= range[1];
}

bool is_overlaping_range(point2d a, point2d b) {
    return !(a[0] > b[1] || b[0] > a[1]);
}

vector<point2d> gen_ranges(point2d a, point2d b) {
    vector<point2d> res;
    if (b[0] < a[0]) {
        if (b[1] < a[1]) {
            // a ...-----------...
            // b .---------.......
            res.push_back({b[0], a[0]-1});
            res.push_back({a[0], b[1]});
            res.push_back({b[1]+1, a[1]});
        } else if (b[1] > a[1]) { 
            // a ...-----------...
            // b .---------------.
            res.push_back({b[0], a[0]-1});
            res.push_back({a[0], a[1]});
            res.push_back({a[1]+1, b[1]});
        } else /*b[1] == a[1] */ {
            // a ...-----------...
            // b .-------------...
            res.push_back({b[0], a[0]-1});
            res.push_back({a[0], a[1]});
        }
    } else if (b[0] > a[0]) {
        if (b[1] < a[1]) {
            // a ...-----------...
            // b ......------.....
            res.push_back({a[0], b[0]-1});
            res.push_back({b[0], b[1]});
            res.push_back({b[1]+1, a[1]});
        } else if (b[1] > a[1]) {
            // a ...-----------...
            // b ......----------.
            res.push_back({a[0], b[0]-1});
            res.push_back({b[0], a[1]});
            res.push_back({a[1]+1, b[1]});
        } else /*b[1] == a[1] */ {
            // a ...-----------...
            // b ......--------...
            res.push_back({a[0], b[0]-1});
            res.push_back(b);
        }
    } else /*b[0] == a[0]*/ {
        if (b[1] < a[1]) {
            // a ...-----------...
            // b ...---------.....
            res.push_back(b);
            res.push_back({b[1]+1, a[0]});
        } else if (b[1] > a[1]) {
            // a ...-----------...
            // b ...-------------.
            res.push_back(a);
            res.push_back({a[1] + 1, b[1]});
        } else /*b[1] == a[1] */ {
            // a ...-----------...
            // b ...-----------...
            res.push_back(b);
        }
    }
    return res;
}

area_t gen_intersection(area_t a, area_t b) {
    vector<point2d> ranges_x = gen_ranges(get<0>(a), get<0>(b));
    vector<point2d> ranges_y = gen_ranges(get<1>(a), get<1>(b));
    vector<point2d> ranges_z = gen_ranges(get<2>(a), get<2>(b));
    for (auto rx : ranges_x) {
        for (auto ry : ranges_y) {
            for (auto rz : ranges_z) {
                if (is_within(rx[0], get<0>(a)) && is_within(ry[0], get<1>(a)) && is_within(rz[0], get<2>(a))
                    && is_within(rx[0], get<0>(b)) && is_within(ry[0], get<1>(b)) && is_within(rz[0], get<2>(b))) {
                        return make_tuple(rx, ry, rz, 0);
                }
            }
        }
    }
    return area_t{};
}

long volume_of(point2d dx, point2d dy, point2d dz) {
    return (dx[1]-dx[0]+1)*(dy[1]-dy[0]+1)*(dz[1]-dz[0]+1);
}

int main(int argc, char** argv) {
    point2d limit_grid = {-50, 50};
    vector<area_t> areas;
    for (auto [cmd, dx, dy, dz] : input) {
        bool bstate = cmd == "on";
        area_t b = make_tuple(dx, dy, dz, bstate ? volume_of(dx, dy, dz) : 0);
        // cout << "next cuboid: " << b << endl;

        vector<area_t> add_areas;

        for (long i = 0; i < areas.size(); ++i) {
            auto& a = areas[i];
            auto [adx,ady,adz,as] = a;
            if (is_overlaping_range(adx, dx) && is_overlaping_range(ady, dy) && is_overlaping_range(adz, dz)) {
                auto new_area = gen_intersection(a, b);
                auto& [ndx,ndy,ndz,ns] = new_area;
                long sign = as < 0 ? 1 : -1;
                ns = sign * volume_of(ndx,ndy,ndz);
                add_areas.push_back(new_area);
            }
        }
        if (bstate) add_areas.push_back(b);

        areas.insert(areas.end(), add_areas.begin(), add_areas.end());
        // cout << "snapshot:\n" << areas << endl << endl;
    } 
    long total_lit = 0;
    for (auto [dx, dy, dz, s] : areas) {
        total_lit += s;
    }
    cout << "total lit: " << total_lit << endl;
    return 0;
}


