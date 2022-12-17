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

using namespace std;

using map_t = vector<vector<int>>;
using point_t = tuple<int,int>;

#define REPEATS 5

istream& operator>>(istream& is, map_t& m) {
    string s;
    while (getline(is, s)) {
        if (s.empty()) break;
        m.emplace_back();
        for (char ch : s) {
            m.back().push_back(ch - '0');
        }
    }
    return is;
}

vector<point_t> reachable_points_from(map_t const& m, point_t from) {
    vector<point_t> r;
    auto [i,j] = from;
    if (i > 0) r.emplace_back(i-1, j);
    if (j > 0) r.emplace_back(i, j-1);
    if (i < m.size()*REPEATS - 1) r.emplace_back(i+1, j);
    if (j < m.front().size()*REPEATS - 1) r.emplace_back(i, j+1);
    return r;
}

int get_point_risk(map_t const& m, point_t p) {
    auto [i,j] = p;
    auto [ni, nj] = make_tuple(i % m.size(), j % m.front().size());
    auto [ri, rj] = make_tuple(i / m.size(), j / m.front().size());
    int added_risk = ri + rj;
    return (m[ni][nj] + added_risk) % 10 + (m[ni][nj] + added_risk) / 10;
}

ostream& operator<<(ostream& os, map_t const& m) {
    for (int i = 0; i < m.size() * REPEATS; ++i) {
        for (int j = 0; j < m.front().size() * REPEATS; ++j) {
            os << get_point_risk(m, {i, j});
        }
        os << "\n";
    }
    return os;
}

int find_min_risk_path(map_t const& m, point_t from, point_t to) {
    map_t riskmap;
    riskmap.resize(m.size() * REPEATS);
    for (auto& row : riskmap) row.resize(m.front().size() * REPEATS, 0);
    map_t visited = riskmap;

    auto cmp = [&riskmap](point_t a, point_t b) {
        auto [ai,aj] = a;
        auto [bi,bj] = b;
        return riskmap[ai][aj] > riskmap[bi][bj];
    };
    priority_queue<point_t, vector<point_t>, decltype(cmp)> q(cmp);
    q.push(from);

    auto [di, dj] = to;
    while (!q.empty()) {
        auto [i,j] = q.top();
        if (i == di && j == dj) break;
        q.pop();
        for (auto [ii, jj] : reachable_points_from(m, {i, j})) {
            int risk = get_point_risk(m, {ii, jj});
            if (riskmap[ii][jj] == 0) { // || riskmap[ii][jj] > riskmap[i][j] + risk) {
                riskmap[ii][jj] = riskmap[i][j] + risk;
                if (visited[ii][jj] >= 0) {
                    q.emplace(ii, jj);
                    visited[ii][jj] = -1;
                }
            }
        }
    }
    return riskmap[di][dj];
}

int main(int argc, char** argv) {
    map_t m;
    cin >> m;
    cout << m;
    cout << "min risk path: " <<
        find_min_risk_path(m,
            make_tuple(0, 0),
            make_tuple(REPEATS*m.size() - 1, REPEATS*m.back().size() - 1)
        )
        << endl;
    return 0;
}