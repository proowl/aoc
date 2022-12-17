
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

using point2d = array<int, 2>;

vector<tuple<string, point2d, point2d, point2d>> input = {
#ifdef REAL_INPUT
#include "input.h"
#else
    { "on", {10, 12}, {10, 12}, {10, 12}},
    { "on", {11, 13},{11, 13},{11, 13}},
    { "off", {9, 11},{9, 11},{9, 11}},
    { "on", {10, 10},{10, 10},{10, 10}},
#endif
};

ostream& operator<<(ostream& os, point2d const& r) {
    for (auto v : r)
        cout << setw(3) << v << " ";
    return os;
}

// ostream& operator<<(ostream& os, vector<int> const& r) {
//     for (auto v : r)
//         cout << setw(3) << v << " ";
//     return os;
// }

// ostream& operator<<(ostream& os, vector<vector<int>> const& r) {
//     for (auto v : r) {
//         os <<  v << "\n";
//     }
//     return os;
// }

bool is_within(int val, point2d range) {
    return val >= range[0] && val <= range[1];
}

int main(int argc, char** argv) {
    point2d limit_grid = {-50, 50};
    array<bool,101*101*101> flat_grid;
    flat_grid.fill(false);
    for (auto& v : input) {
        auto [cmd, xrange, yrange, zrange] = v;
        // cout << cmd << " " << xrange << " " << yrange << " " << zrange << endl;
        bool turn_on = cmd == "on";
        for (int x = xrange[0]; x <= xrange[1]; ++x) {
            if (!is_within(x, limit_grid)) continue;
            for (int y = yrange[0]; y <= yrange[1]; ++y) {
                if (!is_within(y, limit_grid)) continue;
                for (int z = zrange[0]; z <= zrange[1]; ++z) {
                    if (!is_within(z, limit_grid)) continue;
                    int index = (x+50)*101*101 + (y+50)*101 + (z+50);
                    flat_grid[index] = turn_on;
                }
            }
        }
    }
    int total_lit = 0;
    for (auto v : flat_grid)
        if (v) ++total_lit;
    cout << "total lit: " << total_lit << endl;
    return 0;
}
