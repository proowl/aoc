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

// #define TEST
#include "input.h"

static long rolls = 0;

int next_dice() {
    static int val = 1;
    int ret = val;
    if (val > 100) val = 1;
    ++val;
    ++rolls;
    return ret;
}

int main(int argc, char** argv) {
    int player_won = -1;
    map<int, int> scores;
    while (player_won < 0) {
        for (auto& player : player_pos) {
            int rolled = next_dice() + next_dice() + next_dice();
            int pos = player.second + rolled;
            // scores: 1 2 3 4 5 6 7 8 9 10
            // pos:    0 1 2 3 4 5 6 7 8 9
            while (pos > 10) pos -= 10;
            scores[player.first] += pos;
            cout << "p" << player.first
                << " rolled: " << rolled
                << " moveto:" << pos
                << " score: " << scores[player.first] << endl;
            player.second = pos;
            if (scores[player.first] >= 1000) {
                player_won = player.first;
                break;
            }
        }
    }
    cout << "player " << player_won << " won" << endl;
    cout << "total rolls: " << rolls << endl;
    for (auto player : scores) {
        cout << "p" << player.first << " : " << player.second << " answer=" << player.second * rolls << endl;
    }
    return 0;
}