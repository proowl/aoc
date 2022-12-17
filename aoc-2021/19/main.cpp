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

#define MIN_POINTS 12

// istream& operator>>(istream& is, map_t& m) {
//     string s;
//     while (getline(is, s)) {
//         if (s.empty()) break;
//         int pos = s.find_first_of(',');
//         int x = std::stoi(s.substr(0, pos));
//         int y = std::stoi(s.substr(pos+1));
//         set_point(m, x, y);
//     }
//     return is;
// }


using point3d = array<int,3>;

ostream& operator<<(ostream& os, point3d const& r) {
    cout << r[0] << "," << r[1] << "," << r[2];
    return os;
}

ostream& operator<<(ostream& os, vector<point3d> const& r) {
    for (auto v : r) {
        os <<  v << "\n";
    }
    return os;
}

// {-1,  0,  0}, {  0,  0,  1}, { 0,  1,  0}
vector<vector<point3d>> rotations = {
    // fixate z = z
    {{ 1,  0,  0}, { 0,  1,  0}, { 0,  0,  1}},
    {{ 0,  1,  0}, {-1,  0,  0}, { 0,  0,  1}},
    {{-1,  0,  0}, { 0, -1,  0}, { 0,  0,  1}},
    {{ 0, -1,  0}, { 1,  0,  0}, { 0,  0,  1}},
    // fixate z = -z
    {{ 1,  0,  0}, { 0, -1,  0}, { 0,  0, -1}},
    {{ 0, -1,  0}, {-1,  0,  0}, { 0,  0, -1}},
    {{-1,  0,  0}, { 0,  1,  0}, { 0,  0, -1}},
    {{ 0,  1,  0}, { 1,  0,  0}, { 0,  0, -1}},
    // fixate z = y
    {{ 1,  0,  0}, { 0,  0, -1}, { 0,  1,  0}},
    {{ 0,  0, -1}, {-1,  0,  0}, { 0,  1,  0}},
    {{-1,  0,  0}, { 0,  0,  1}, { 0,  1,  0}},
    {{ 0,  0,  1}, { 1,  0,  0}, { 0,  1,  0}},
    // fixate z = -y
    {{ 1,  0,  0}, { 0,  0,  1}, { 0, -1,  0}},
    {{ 0,  0, -1}, { 1,  0,  0}, { 0, -1,  0}},
    {{-1,  0,  0}, { 0,  0, -1}, { 0, -1,  0}},
    {{ 0,  0,  1}, {-1,  0,  0}, { 0, -1,  0}},
    // fixate z = x
    {{ 0,  1,  0}, { 0,  0,  1}, { 1,  0,  0}},
    {{ 0,  0, -1}, { 0,  1,  0}, { 1,  0,  0}},
    {{ 0, -1,  0}, { 0,  0, -1}, { 1,  0,  0}},
    {{ 0,  0,  1}, { 0, -1,  0}, { 1,  0,  0}},
    // fixate z = -x
    {{ 0,  1,  0}, { 0,  0, -1}, {-1,  0,  0}},
    {{ 0,  0, -1}, { 0, -1,  0}, {-1,  0,  0}},
    {{ 0, -1,  0}, { 0,  0,  1}, {-1,  0,  0}},
    {{ 0,  0,  1}, { 0,  1,  0}, {-1,  0,  0}},
};

vector<point3d> apply_rotation(const vector<point3d>& points, const vector<point3d>& rotate) {
    vector<point3d> res;
    auto [ax,bx,cx] = rotate[0];
    auto [ay,by,cy] = rotate[1];
    auto [az,bz,cz] = rotate[2];
    for (auto p : points) {
        res.push_back({ax*p[0] + bx*p[1] + cx*p[2], ay*p[0] + by*p[1] + cy*p[2], az*p[0] + bz*p[1] + cz*p[2]});
    }
    return res;
}

// size = scanner1.size
//   [i].size = scanner2.size
vector<vector<point3d>> find_distances(vector<point3d> const& scanner1, vector<point3d> const& scanner2) {
    vector<vector<point3d>> dist;
    dist.resize(scanner1.size());
    for (int i = 0; i < scanner1.size(); ++i) {
        for (int j = 0; j < scanner2.size(); ++j) {
            dist[i].push_back({scanner2[j][0] - scanner1[i][0], scanner2[j][1] - scanner1[i][1], scanner2[j][2] - scanner1[i][2]});
        }
    }
    return dist;
}

point3d find_overlap(vector<vector<point3d>> const& distances) {
    point3d dist = {0, 0, 0};
    map<point3d, int> freq;
    int max_freq = 0;
    for (int i = 0; i < distances.size(); ++i) {
        for (int j = 0; j < distances[i].size(); ++j) {
            auto cur_dist = distances[i][j];
            freq[cur_dist] += 1;
            if (freq[cur_dist] > max_freq) {
                max_freq = freq[cur_dist];
                dist[0] = cur_dist[0];
                dist[1] = cur_dist[1];
                dist[2] = cur_dist[2];
            }
        }
    }
    if (max_freq >= MIN_POINTS) {
#ifdef DEBUG
        cout << "overlap FOUND\n";
#endif
        return dist;
    }
#ifdef DEBUG
    cout << "overlap not found\n";
#endif
    return point3d{0, 0, 0};
}

int main(int argc, char** argv) {
    int scanner_id = -1;
    map<int, vector<point3d>> scanners;
    while (!cin.eof()) {
        string s;
        getline(cin,s);
        if (s.empty())
            continue;
        if (s.substr(0,3) == "---") {
            ++scanner_id;
            continue;
        }
        int comma1 = s.find_first_of(',');
        int comma2 = s.find_last_of(',');
        int x = stoi(s.substr(0, comma1));
        int y = stoi(s.substr(comma1 + 1, comma2 - comma1));
        int z = stoi(s.substr(comma2 + 1));
        // cout << "parsed " << s << " to " << x << "," << y << "," << z << endl;
        scanners[scanner_id].push_back({x,y,z});
    }

    map<int, set<int>> overlapping_scanners;

    vector<point3d>& scanner1 = scanners[0];
    bool folded = true;
    vector<point3d> scanner_pos = {{0, 0, 0}};
    set<int> folded_ids;
    while (folded) {
        folded = false;
        for (int j = 1; j <= scanner_id; ++j) {
            vector<point3d>& scanner2 = scanners[j];
            if (folded_ids.find(j) != folded_ids.end()) continue;
            for (auto rotate : rotations) {
                vector<point3d> rotated = apply_rotation(scanner2, rotate);
                auto distances = find_distances(rotated, scanner1);
                auto dist = find_overlap(distances);
                if (tie(dist[0], dist[1], dist[2]) != make_tuple(0, 0, 0)) {
#ifdef DEBUG
                    cout << "found match between\n" << scanner1 << "\n\nand (#" << j << ")\n\n" << rotated 
                        << " common dist: " << dist << endl;
#endif
                    scanner2 = rotated;
                    for (int pos = 0; pos < rotated.size(); ++pos) {
                        bool seen = false;
                        for (auto& d : distances[pos]) {
                            if (tie(d[0], d[1], d[2]) == tie(dist[0], dist[1], dist[2])) {
                                seen = true;
                                break;
                            }
                        }
                        auto moved = rotated[pos];
                        moved[0] += dist[0];
                        moved[1] += dist[1];
                        moved[2] += dist[2];
                        if (seen) {
#ifdef DEBUG
                            cout << "[dup]: " << moved << " was " << scanner2[pos] << endl;
#endif
                        } else {
#ifdef DEBUG
                            cout << "[add]: " << moved << " was " << scanner2[pos] << endl;
#endif
                            scanner1.push_back(moved);
                        }
                    }
                    folded_ids.insert(j);
                    scanner_pos.push_back(dist);
                    folded = true;
                    break;
                }
            }
        }
    }

    int total_nodes = scanner1.size();
    cout << "unique nodes: " << total_nodes << endl;

    int max_dist = 0;
    for (int i = 0; i < scanner_pos.size(); ++i) {
        for (int j = 0; j < scanner_pos.size(); ++j) {
            int dist = abs(scanner_pos[j][0] - scanner_pos[i][0]) + abs(scanner_pos[j][1] - scanner_pos[i][1]) + abs(scanner_pos[j][2] - scanner_pos[i][2]);
            max_dist = max(dist, max_dist);
        }
    }
    cout << "max Manhattan distance: " << max_dist << endl;
    return 0;
}
