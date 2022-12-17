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
#include <thread>
#include <atomic>
#include <regex>

using namespace std;

using grid_t = vector<string>;

ostream& operator<<(ostream& os, grid_t const& grid) {
    for(auto row : grid)
        os << row << "\n";
    return os;
}

int main (int argc, char** argv) {
    grid_t grid;
    string s;
    while (getline(cin, s)) {
        grid.push_back(s);
    }

    int steps = 0;
    bool changed = true;
    while (changed) {
        changed = false;
        grid_t next = grid;
        for (int i = 0; i < grid.size(); ++i) {
            for (int j = 0; j < grid[i].size(); ++j) {
                int jnext = (j + 1) %  grid[i].size();
                if (grid[i][j] == '>' && grid[i][jnext] == '.') {
                    char ch = next[i][j];
                    next[i][j] = next[i][jnext];
                    next[i][jnext] = ch;
                    changed = true;
                }
            }
        }
        grid = next;
        for (int i = 0; i < grid.size(); ++i) {
            int inext = (i + 1) %  grid.size();
            for (int j = 0; j < grid[i].size(); ++j) {
                if (grid[i][j] == 'v' && grid[inext][j] == '.') {
                    char ch = next[i][j];
                    next[i][j] = next[inext][j];
                    next[inext][j] = ch;
                    changed = true;
                }
            }
        }
        grid = next;
        ++steps;
        // cout << "After step #" << steps << endl;
        // cout << grid << "\n";
    }

    cout << "steps: " << steps << endl;
    return 0;
}