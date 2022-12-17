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

int main(int argc, char** argv) {
    // target area: x=20..30, y=-10..-5
    // array<int,2> x_range = { 20, 30};
    // array<int,2> y_range = {-10, -5};

    // target area: x=209..238, y=-86..-59
    array<int,2> x_range = {209, 238};
    array<int,2> y_range = {-86, -59};

    // x=3000..-3010, y=-1010..-1000
    // array<int,2> x_range = {3000, 3010};
    // array<int,2> y_range = {-1010, -1000};

    auto fits_inside = [](int d0, int n, array<int,2> const& range) {
        int dn = d0 - (n-1);
        int pos = n * (d0 + dn) / 2;
        return pos >= range[0] && pos <= range[1];
    };

    int max_y = abs(y_range[0]) > abs(y_range[1]) ? abs(y_range[0]) : abs(y_range[1]);
    int max_x = x_range[0] > x_range[1] ? x_range[0] : x_range[1];
    int min_y = y_range[0] < y_range[1] ? y_range[0] : y_range[1];
    int max_n = max(abs(min_y)*2, abs(x_range[0]) > abs(x_range[1]) ? abs(x_range[0]) : abs(x_range[1]));

    int total_combinations = 0;
    int max_y_pos = 0;
    int simulations = 0;
    int simulations_with_n = 0;

    for (int dy0 = min_y; dy0 <= max_y; ++dy0) {
        for (int dx0 = 0; dx0 <= max_x; ++dx0) {
            ++simulations;
            for (int n = 1; n <= max_n; ++n) {
                ++simulations_with_n;
                if (fits_inside(dy0, n, y_range)) {
                    if (fits_inside(dx0, min(n, dx0), x_range)) {
                        max_y_pos = max(max_y_pos, (dy0 * (dy0 + (dy0 - dy0 + 1)) / 2));
                        ++total_combinations;
                        cout
                            << "final solution at " << n
                            << " dy=" << dy0
                            << " dx=" << dx0
                            << " max_y=" << max_y_pos
                            << " total_combinations=" << total_combinations
                            << endl;
                        break;
                    }
                }
            }
        }
    }
    cout << "sims: " << simulations << " or " << simulations_with_n << endl;
    return 0;
}