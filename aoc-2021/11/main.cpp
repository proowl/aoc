#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <iomanip>

struct Point {
    int x = 0;
    int y = 0;
};

void debug_print(const char* header, const std::vector<std::vector<int>>& m) {
#ifdef DEBUG
    std::cout << header << "\n";
    for (auto& r : m) {
        for (int v : r) {
            std::cout << std::setw(3) << v;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
#endif
}

int main(int argc, char** argv) {
    int steps = std::atoi(argv[1]);
    std::vector<std::vector<int>> m;
    while (!std::cin.eof()) {
        std::string s;
        std::cin >> s;
        if (std::cin.bad()) break;
        m.emplace_back();
        auto& row = m.back();
        for (char c: s) {
            row.emplace_back(c - '0');
        }
    }
    debug_print("init", m);
    int it = 0;
    int total_flashes = 0;
    while (++it <= steps) {
#ifdef DEBUG
        std::cout << "step " << it << "\n";
#endif
        std::queue<Point> flashes;
        int flashes_this_step = 0;
        for (int i = 0; i < m.size(); ++i) {
            auto& r = m[i];
            for (int j = 0; j < r.size(); ++j) {
                if (++r[j] == 10) {
                    flashes.push({i,j});
                }
            }
        }
        debug_print("[inc]", m);
        while (!flashes.empty()) {
            auto p = flashes.front();
            flashes.pop();
            ++total_flashes;
            ++flashes_this_step;
#ifdef DEBUG
            std::cout << p.x << "," << p.y << " flashed" << std::endl;
#endif
            for (int i : {-1, 0, 1}) {
                for (int j : {-1, 0, 1}) {
                    if (i == 0 && j == 0) continue;
                    if (p.x+i < 0
                    || p.y+j < 0
                    || p.x+i >= m.size()
                    || p.y+j >= m.front().size()) continue;
                    if (++m[p.x+i][p.y+j] == 10) {
                        flashes.push({p.x+i, p.y+j});
                    }
                }
            }
            debug_print("after flash", m);
        }
        for (auto& r : m) {
            for (int& v : r) {
                if (v > 9) v = 0;
            }
        }
        debug_print("step end", m);
        if (flashes_this_step == m.size() * m.front().size()) {
            std::cout << "first all flashes at step " << it << std::endl;
            break;
        }
    }
    std::cout << "total flashes: " << total_flashes << std::endl;
    return 0;
}