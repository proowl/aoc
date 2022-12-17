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

#define TEST
#include "input.h"

#define WIN_AT 21
// #define WIN_AT 12

array<int, 7> dice_sums      = {3,4,5,6,7,8,9};
array<int, 7> dice_sum_paths = {1,3,6,7,6,3,1};
array<int, 10> positions = {10,1,2,3,4,5,6,7,8,9};

ostream& operator<<(ostream& os, vector<int> const& r) {
    for (auto v : r)
        cout << setw(3) << v << " ";
    return os;
}

ostream& operator<<(ostream& os, vector<vector<int>> const& r) {
    for (auto v : r) {
        os <<  v << "\n";
    }
    return os;
}

map<int,long> win_count;

struct Player {
    int pos = 0;
    int score = 0;
};

map<int,long> count_wins(map<int, Player> players, int step, long paths = 1) {
    map<int,long> win_count;
    for (auto p : players) {
        win_count[p.first] = 0;
    }
    int player = step % 2;
    int current_pos = players[player].pos;
    int current_sum = players[player].score;
    for (int di = 0; di < dice_sums.size(); ++di) {
        int next_pos = positions[(current_pos + dice_sums[di])%10];
        int next_sum = next_pos + current_sum;
        if (next_sum >= WIN_AT) {
            win_count[player] += paths * dice_sum_paths[di];
        } else {
            map<int, Player> p = players;
            p[player].pos = next_pos;
            p[player].score = next_sum;
            auto subwin_this_path = count_wins(p, step + 1, paths * dice_sum_paths[di]);
            for (auto kv : subwin_this_path) {
                win_count[kv.first] += kv.second;
            }
        }
    }
    return win_count;
}

int main(int argc, char** argv) {
    // map<int,int> freq;
    // for (int i = 1; i <= 3; ++i) {
    //     for (int j = 1; j <= 3; ++j) {
    //         for (int k = 1; k <= 3; ++k) {
    //             freq[i+j+k] +=1;
    //         }
    //     }
    // }
    // for (auto kv : freq) cout << kv.first << " " << kv.second << endl;
    // map<int, Player> players = {{0, {4, 0}}, {1, {8, 0}}};
    map<int, Player> players = {{0, {3, 0}}, {1, {5, 0}}};
    auto wins = count_wins(players, 0);
    cout << "p1: " << wins[0] << " p2: " << wins[1] << endl;
    return 0;
}
