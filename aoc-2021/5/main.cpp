#include <iostream>
#include <string>
#include <vector>
#include <regex>

struct Point {
    int x = 0;
    int y = 0;

    bool operator==(Point const& o) const {
        return x == o.x && y == o.y;
    }

    bool operator!=(Point const& o) const {
        return x != o.x || y != o.y;
    }
};

using grid_t = std::vector<std::vector<int>>;

void inc_point(grid_t& g, Point p) {
    if (p.x >= g.size()) {
        g.resize(p.x + 1);
    }
    auto& row = g[p.x];
    if (p.y >= row.size()) {
        row.resize(p.y + 1, 0);
    }
    row[p.y] += 1;
}

void update_grid(grid_t& grid, Point p1, Point p2) {
    int dx = p2.x > p1.x ? 1 : (p2.x < p1.x ? -1 : 0);
    int dy = p2.y > p1.y ? 1 : (p2.y < p1.y ? -1 : 0);
    // uncomment for part 1
    // if (dx == 0 || dy == 0) {
        Point p = p1;
        inc_point(grid, p);
        while (p != p2) {
            p.x += dx;
            p.y += dy;
            inc_point(grid, p);
        }
    // }
}

void print_grid(grid_t const& g) {
    for (auto& r : g) {
        for (auto v : r) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}

int main(int argc, char** argv) {
    grid_t grid;
    std::regex rgx("^([0-9]+),([0-9]+)\\s+->\\s+([0-9]+),([0-9]+)");

    while (!std::cin.eof()) {
        int x,y;
        std::string s;
        std::getline(std::cin, s);
        std::smatch m;
        if(std::regex_search(s, m, rgx)) {
            Point p1,p2;
            p1.x = std::stoi(m[1].str());
            p1.y = std::stoi(m[2].str());
            p2.x = std::stoi(m[3].str());
            p2.y = std::stoi(m[4].str());
            // std::cout << 
            //     "p1(" << p1.x << "," << p1.y << ") "
            //     "p2(" << p2.x << "," << p2.y << ") "
            //     " str=" << s << std::endl;
            update_grid(grid, p1, p2);
        } else {
            std::cerr << "INVALID STRING: " << s << "\n";
        }
    }

    int danger_count = 0;
    for (auto& row : grid) {
        for (auto v : row) {
            if (v > 1) ++danger_count;
        }
    }

    // print_grid(grid);
    std::cout << "danger_count=" << danger_count << std::endl;
    return 0;
}