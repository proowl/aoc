
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

constexpr char kEmptySlot = '.';
constexpr char kReservedSlot = '_';
constexpr char kUnknown = ' ';
constexpr char kWall = '#';

enum CellType {
    kHallCell,
    kRoomCell,
    kInvalidCell,
};

struct Cell {
    CellType type = kInvalidCell;
    char value = kUnknown;
};

using grid_t = vector<vector<Cell>>;
using point2d = array<int, 2>;

ostream& operator<<(ostream& os, Cell const& v) {
    os << v.value;
    return os;
}

ostream& operator<<(ostream& os, vector<Cell> const& r) {
    for (auto v : r) os << v;
    return os;
}

ostream& operator<<(ostream& os, grid_t const& r) {
    for (auto v : r) os <<  v << "\n";
    return os;
}

ostream& operator<<(ostream& os, point2d const& r) {
    os << r[0] << "," << r[1];
    return os;
}

int min_price = numeric_limits<int>::max();

map<char, int> cost = {
    {'A', 1},
    {'B', 10},
    {'C', 100},
    {'D', 1000},
};

map<char, int> dest_room = {
    {'A', 3},
    {'B', 5},
    {'C', 7},
    {'D', 9},
};
    
bool can_move_through(Cell cell) {
    return (cell.type == kHallCell || cell.type == kRoomCell)
        && (cell.value == kReservedSlot || cell.value == kEmptySlot);
}

bool can_occupy(Cell cell) {
    return (cell.type == kHallCell || cell.type == kRoomCell) && cell.value == kEmptySlot;
}

bool is_amphipod(char v) {
    return cost.find(v) != cost.end();
}

// returns {dest point, cost}
vector<tuple<point2d, int>> find_accessible_dest(grid_t grid, point2d at) {
    auto& cell = grid[at[0]][at[1]];
    char amphipod = cell.value;
    vector<tuple<point2d, int>> res;

    // Search path out of the room.
    int out_of_the_room_cost = 0;
    if (cell.type == kRoomCell) {
        int i = at[0]-1;
        for (; i > 1; --i) {
            if (!can_move_through(grid[i][at[1]])) break;
        }
        bool path_to_hall_open = i == 1;
        if (!path_to_hall_open) {
            // cout << amphipod << " path to hall is not accessible\n";
            return {};
        }
        out_of_the_room_cost = at[0] - 1;
    }

    // Search accessible tiles in the hallway.
    int left_min = at[1];
    int right_max = at[1];
    while (can_move_through(grid[1][left_min - 1])) --left_min;
    while (can_move_through(grid[1][right_max + 1])) ++right_max;
    if (cell.type == kRoomCell) {
        // cout << amphipod << " searching path froom " << left_min << " to " << right_max << endl;
        for (int i = left_min; i <= right_max; ++i) {
            if (!can_occupy(grid[1][i])) continue;
            int hallway_cost = abs(i - at[1]);
            res.push_back(make_tuple(point2d{1, i}, cost[amphipod] * (out_of_the_room_cost + hallway_cost)));
        }
    }

    // Try move to dest room.
    int jdest = dest_room[amphipod];
    if (jdest >= left_min && jdest <= right_max) {
        bool is_room_good = true;
        for (int i = 2; i < grid.size() - 1 && is_room_good; ++i) {
            if (can_move_through(grid[i][jdest]) || grid[i][jdest].value == amphipod) {
                continue;
            } else {
                is_room_good = false;
            }
        }
        if (is_room_good) {
            int pos = grid.size() - 2;
            while (!can_occupy(grid[pos][jdest])) --pos;
            int hallway_cost = abs(jdest - at[1]);
            int room_cost = pos - 1;
            res.insert(res.begin(), make_tuple(point2d{pos, jdest}, cost[amphipod] * (out_of_the_room_cost + hallway_cost + room_cost)));
        }
    }
    return res;
}

bool is_at_final_point(char ch, const grid_t& grid, const point2d& at) {
    auto iroom = dest_room.find(ch);
    if (iroom->second != at[1]) return false;
    // Check below only destination amphipods.
    for (int i = at[0]+1; i < grid.size() - 1; ++i) {
        if (grid[i][at[1]].value != ch)
            return false;
    }
    return true;
}

bool is_final_state(const grid_t& grid) {
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid.front().size(); ++j) {
            auto& cell = grid[i][j];
            if (cell.type == kInvalidCell) continue;

            if (is_amphipod(cell.value)) {
                return is_at_final_point(cell.value, grid, {i, j});
            }
        }
    }
    return true;
};

void find_min_price_path(grid_t grid, int current_cost = 0, stack<tuple<char,point2d,point2d,int>> moves = {}) {
    if (current_cost >= min_price) {
        return;
    }
#ifdef DEBUG
    cout << "\ncost=" << current_cost << endl << grid;
#endif
    if (is_final_state(grid)) {
        cout << "min_price=" << min_price << " current_cost=" << current_cost << " STONKS\n";
#ifdef DEBUG
        cout << "path:\n";
        while (!moves.empty()) {
            auto [who, from, where, cost] = moves.top();
            cout << who << " from " << from << " to " << where << " at cost=" << cost << endl;
            moves.pop();
        }
#endif
        if (current_cost < min_price) min_price = current_cost;
        return;
    }

    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid.front().size(); ++j) {
            auto& cell = grid[i][j];
            if (cell.type == kInvalidCell) continue;

            if (is_amphipod(cell.value)) {
                if (is_at_final_point(cell.value, grid, {i, j})) {
                    continue;
                }
#ifdef DEBUG
                cout << "searching paths for " << cell.value << " from " << i << "," << j << endl;
#endif
                for (auto [pos, cost] : find_accessible_dest(grid, {i, j})) {
#ifdef DEBUG
                    cout << "found path to " << pos << " cost=" << cost << endl;
#endif
                }
                for (auto [pos, cost] : find_accessible_dest(grid, {i, j})) {
                    grid_t next_grid = grid;
                    next_grid[i][j].value = kEmptySlot;
                    next_grid[pos[0]][pos[1]].value = cell.value;
                    auto next_moves = moves;
#ifdef DEBUG
                    next_moves.push(make_tuple(cell.value, point2d{i,j}, pos, cost));
#endif
                    find_min_price_path(next_grid, current_cost + cost, next_moves);
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    grid_t grid;
    Cell wall {.type=kInvalidCell, .value=kWall};
    // Build hallway.
    // #############
    // #...........#
    grid.emplace_back();
    grid.back().resize(13, wall);
    grid.emplace_back();
    grid.back().push_back(wall);
    grid.back().resize(12, Cell{.type=kHallCell, .value=kEmptySlot});
    grid.back()[3].value = kReservedSlot;
    grid.back()[5].value = kReservedSlot;
    grid.back()[7].value = kReservedSlot;
    grid.back()[9].value = kReservedSlot;
    grid.back().push_back(wall);

    // Build init rooms states.
    // int rooms_depth = 2;
    int rooms_depth = 4;
    for (int i = 0; i < rooms_depth; ++i) {
        grid.emplace_back();
        Cell filler{.type=kInvalidCell, .value=i == 0 ? kWall : kUnknown};
        grid.back().push_back(filler);
        grid.back().push_back(filler);
        for (int j = 0; j < 4; ++j) {
            grid.back().push_back(wall);
            grid.back().push_back(Cell{.type=kRoomCell, .value=kEmptySlot});
        }
        grid.back().push_back(wall);
        grid.back().push_back(filler);
        grid.back().push_back(filler);
    }

    // part 2
    grid[2][3].value = 'C'; grid[2][5].value = 'B'; grid[2][7].value = 'D'; grid[2][9].value = 'D';
    grid[3][3].value = 'D'; grid[3][5].value = 'C'; grid[3][7].value = 'B'; grid[3][9].value = 'A';
    grid[4][3].value = 'D'; grid[4][5].value = 'B'; grid[4][7].value = 'A'; grid[4][9].value = 'C';
    grid[5][3].value = 'B'; grid[5][5].value = 'C'; grid[5][7].value = 'A'; grid[5][9].value = 'A';

    // part 1
    // grid[2][3].value = 'C'; grid[2][5].value = 'B'; grid[2][7].value = 'D'; grid[2][9].value = 'D';
    // grid[3][3].value = 'B'; grid[3][5].value = 'C'; grid[3][7].value = 'A'; grid[3][9].value = 'A';

    // test 1
    // grid[2][3].value = 'B';
    // grid[2][5].value = 'A';

    // test 2
    // grid[2][5].value = 'A';

    // Add bottom wall.
    grid.emplace_back();
    grid.back().resize(2, Cell{.type=kInvalidCell, .value=kUnknown});
    grid.back().resize(11, wall);
    grid.back().push_back(Cell{.type=kInvalidCell, .value=kUnknown});


    cout << "initial state\n";
    cout << grid << endl;
    find_min_price_path(grid);
    cout << "min price path: " << min_price << endl;
    return 0;
}
