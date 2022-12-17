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

// struct PairTree {
//     PairTree* left = nullptr;
//     PairTree* right = nullptr;

//     int val;

//     void addTree(PairTree* other) {
//     }
// };

// PairTree* parseTree(string const& s) {
//     PairTree* node = new PairTree(); 
//     for 
//     return nullptr;
// }

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

constexpr int kStart = -1;
constexpr int kEnd = -2;
constexpr int kComma = -3;

static const map<char, int> symbols = {
    {'[', kStart},
    {']', kEnd},
    {',', kComma},
};


ostream& operator<<(ostream& os, vector<int> const& vals) {
    for (auto v : vals) {
        if (v < 0) {
            switch(v) {
                case kStart: cout << "["; break;
                case kEnd: cout << "]"; break;
                case kComma: cout << ","; break;
            }
        } else {
            cout << v;
        }
    }
    return os;
}

vector<int> split_val(int val) {
    vector<int> s;
    if (val > 9) {
        s.push_back(kStart);
        s.push_back(val / 2);
        s.push_back(kComma);
        s.push_back(1.0*val / 2 + 0.5);
        s.push_back(kEnd);
    } else {
        s.push_back(val);
    }
    return s;
}

tuple<bool, vector<int>> explode(vector<int> input) {
    vector<int> exploded;
    int depth = 0;
    int add_left = -1;
    int add_right = -1;
    int pos = -1;
    for (int i = 0; i < input.size(); ++i) {
        int val = input[i];
        switch (val) {
            case kStart: {
                ++depth;
                if (depth >= 5 && pos < 0 && input[i+4] == kEnd) {
                    // +0 [
                    // +1 number
                    // +2 ,
                    // +3 number
                    // +4 ]
                    add_left = input[i+1];
                    add_right = input[i+3];
                    pos = i;
                    i += 4;
                    cout << "exploded {:" << pos << ",(" << add_left << "," << add_right << ")}\n";
                    // explode left
                    for (int j = exploded.size() - 1; j >= 0; --j) {
                        int val = exploded[j];
                        if (val >= 0) {
                            exploded[j] += add_left;
                            break;
                        }
                    }
                    exploded.push_back(0);
                    --depth;
                    continue;
                } else {
                    exploded.push_back(val);
                }
                break;
            }
            case kEnd: {
                --depth;
                [[fallthrough]];
            }
            case  kComma: {
                exploded.push_back(val);
                break;
            }
            default /*number*/: {
                if (pos >= 0 && add_right > 0) {
                    // explode right
                    exploded.push_back(val + add_right);
                    add_right = 0;
                } else {
                    exploded.push_back(val);
                }
            }
        }
    }
    return make_tuple(pos > 0, exploded);
}

tuple<bool, vector<int>> split(vector<int> input) {
    vector<int> str;
    int pos = -1;
    for (int i = 0; i < input.size(); ++i) {
        int val = input[i];
        switch (val) {
            case kComma:
                [[fallthrough]];
            case kStart:
                [[fallthrough]];
            case kEnd:
                str.push_back(val);
                break;
            default /*number*/: {
                if (pos < 0 && val > 9) {
                    pos = i;
                    auto after_split = split_val(val);
                    cout << "split {" << val << " -> " << after_split << "}\n";
                    str.insert(str.end(), after_split.begin(), after_split.end());
                } else {
                    str.push_back(val);
                }
            }
        }
    }
    return make_tuple(pos > 0, str);
}

vector<int> convert_to_list(string s) {
    vector<int> r;
    for (char ch : s) {
        auto it = symbols.find(ch);
        if (it != symbols.end()) {
            r.push_back(it->second);
        } else {
            r.push_back(ch - '0');
        }
    }
    return r;
}

int fold(int left, int right) {
    return 3*left + 2 * right;
}

int compute_magnitude(vector<int> const& v) {
    vector<int> v2 = v;
    bool folded = true;
    while (folded) {
        folded = false;
        for (int i = 0; i < v2.size(); ++i) {
            if (v2[i] == kStart && v2[i+4] == kEnd) {
                folded = true;
                // cout << "folding at " << i << "\n";
                // cout << v2 << "\n";
                v2[i] = fold(v2[i+1], v2[i+3]);
                v2.erase(v2.begin()+i+1, v2.begin()+i+5);
                // cout << v2 << "\n";
            }
        }
    }
    cout << v2 << endl;
    return v2[0];
}

int main(int argc, char** argv) {
    string s;
    vector<vector<int>> all_input;
    while(getline(cin, s)) {
        all_input.push_back(convert_to_list(s));
    }

    vector<int> result;
    // P1
    for (auto& converted : all_input) {
        if (result.empty()) {
            result = converted;
        } else {
            result.insert(result.begin(), kStart);
            result.insert(result.end(), kComma);
            result.insert(result.end(), converted.begin(), converted.end());
            result.insert(result.end(), kEnd);
        }
        cout << result << endl;
        auto [updated, upd_s] = explode(result);
        while (updated) {
            while (updated) {
                result = upd_s;
                cout << "" << upd_s << endl;
                tie(updated, upd_s) = explode(result);
            }
            tie(updated, upd_s) = split(result);
        }
        // split and try explode again;
        cout << "final:" << result << endl;
    }
    cout << "magnitude: ";
    compute_magnitude(result);

    // P2
    cout << "=======================\n";
    cout << "P2\n";
    cout << "=======================\n";
    int max_magnitude = 0;
    for (int i = 0; i < all_input.size(); ++i) {
        for (int j = 0; j < all_input.size(); ++j) {
            vector<int> result = all_input[i];
            result.insert(result.begin(), kStart);
            result.insert(result.end(), kComma);
            result.insert(result.end(), all_input[j].begin(), all_input[j].end());
            result.insert(result.end(), kEnd);

            auto [updated, upd_s] = explode(result);
            while (updated) {
                while (updated) {
                    result = upd_s;
                    cout << "" << upd_s << endl;
                    tie(updated, upd_s) = explode(result);
                }
                tie(updated, upd_s) = split(result);
            }
            // split and try explode again;
            cout << "final:" << result << endl;
            int magnitude = compute_magnitude(result);
            max_magnitude = max(max_magnitude, magnitude);
        }
    }
    cout << "max magnitude: " << max_magnitude << endl;
    return 0;
}