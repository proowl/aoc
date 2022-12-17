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

using namespace std;

using map_t = vector<vector<bool>>;

void set_point(map_t& m, int x, int y) {
    if (m.size() < y + 1) {
        m.resize(y + 1);
    }

    int row_size = m[0].size() > x + 1 ? m[0].size() : x + 1;
    for (int i = 0; i < m.size(); ++i) {
        if (row_size != m[i].size()) {
            m[i].resize(row_size);
        }
    }

    m[y][x] = true;
}

bool get_point(map_t& m, int x, int y) {
    if (y < 0 || y > m.size()) return false;
    if (x < 0 || x > m[0].size()) return false;
    return m[y][x];
}

istream& operator>>(istream& is, map_t& m) {
    string s;
    while (getline(is, s)) {
        if (s.empty()) break;
        int pos = s.find_first_of(',');
        int x = std::stoi(s.substr(0, pos));
        int y = std::stoi(s.substr(pos+1));
        set_point(m, x, y);
    }
    return is;
}

ostream& operator<<(ostream& os, map_t const& m) {
    for (auto& r: m) {
        for (auto v : r) {
            os << (v ? '#' : '.');
        }
        os << "\n";
    }
    return os;
}

void fold_matrix(istream& is, map_t& m) {
    string s;
    while (getline(is, s)) {
        int v = std::stoi(s.substr(s.find_first_of('=') + 1));
        bool along_y = s[s.find_first_of('=') - 1] == 'y';
        map_t res;
        if (along_y) {
            res.resize(m.size() - v - 1);
            for (auto& r: res) r.resize(m[0].size());
            int max_y = m.size();
            int offset = max_y / 2 - v;
            for (int y = 0; y < res.size(); ++y) {
                for (int x = 0; x < res[y].size(); ++x) {
                    //  _ _ _|# # #|# . # # # #|
                    // |# # # # # #|
                    res[y][x] = get_point(m, x, - offset + y) || get_point(m, x, max_y - y - 1);
                }
            }
            // std::cout << "result:\n" << res << std::endl;
        } else {
            res.resize(m.size());
            for (auto& r: res) r.resize(m[0].size() - v - 1);
            int max_x = m[0].size();
            int offset = max_x/2 - v;
            for (int y = 0; y < res.size(); ++y) {
                for (int x = 0; x < res[y].size(); ++x) {
                    res[y][x] = get_point(m, - offset + x, y) || get_point(m, max_x - x - 1, y);
                }
            }
            // std::cout << "result:\n" << res << std::endl;
        }
#ifdef DEBUG
        std::cout << "after fold: " << s << "\n" << res << std::endl;
#endif
        m = res;
#ifdef FIRST_PASS
        break;
#endif
    }
}

int main(int argc, char** argv) {
    map_t m;
    std::cin >> m;
#ifdef DEBUG
    std::cout << m << std::endl;
#endif
    fold_matrix(std::cin, m);
#ifdef DEBUG
    std::cout << "after fold\n" << m << std::endl;
#endif

    int points = 0;
    for (auto& r: m) {
        for (auto v : r) {
            if (v) ++points;
        }
    }
    std:cout << "points count: " << points << std::endl;
    return 0;
}