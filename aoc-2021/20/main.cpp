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

using matrix = vector<vector<bool>>;
using enh_list = vector<bool>;
using point2d = array<int, 2>;

void operator>>(string const& s, vector<bool>& vals) {
    for (char ch : s) {
        switch (ch) {
            case '.': vals.push_back(false); break;
            case '#': vals.push_back(true); break;
            default:
                cerr << "Unsupported enh list character: " << ch << endl;
                exit(1);
        }
    }
}

istream& operator>>(istream& is, vector<bool>& vals) {
    string s;
    while (getline(is, s)) {
        if (s.empty()) break;
        s >> vals;
    }
    return is;
}

istream& operator>>(istream& is, matrix& m) {
    string s;
    while (getline(cin, s)) {
        vector<bool> vals;
        s >> vals;
        m.push_back(vals);
    }
    return is;
}

ostream& operator<<(ostream& os, vector<bool> const& r) {
    for (auto v : r) {
        os << (v ? '#' : '.');
    }
    return os;
}

ostream& operator<<(ostream& os, const matrix& m) {
    for (auto& r : m) {
        cout << r << "\n";
    }
    return os;
}

bool inf_bit_value = false;

bool get_ij(const matrix& input, int i, int j) {
    if (i < 0 || j < 0)
        return inf_bit_value;
    if (i >= input.size() || j >= input[i].size())
        return inf_bit_value;
    return input[i][j];
}

bool resolve_pixel(const matrix& input, int i, int j, const enh_list& enh) {
    constexpr array<point2d, 9> window = {
        point2d{-1,-1},
        point2d{-1, 0},
        point2d{-1, 1},
        point2d{ 0,-1},
        point2d{ 0, 0},
        point2d{ 0, 1},
        point2d{ 1,-1},
        point2d{ 1, 0},
        point2d{ 1, 1},
    };
    int enh_offset = 0;
    for (const auto& p: window) {
        enh_offset = enh_offset << 1;
        if (get_ij(input, i + p[0], j + p[1])) enh_offset += 1;
    }
    return enh[enh_offset];
}

matrix enhance(const matrix& input, const enh_list& enh) {
    matrix res;
    res.resize(input.size() + 4);
    int input_rows = input.size();
    int input_cols = input.front().size();
    for (int i = 0; i < input_rows + 4; ++i) {
        for (int j = -2; j < input_cols + 2; ++j) {
            res[i].push_back(resolve_pixel(input, i-2, j, enh));
        }
    }
    return res;
}

int main(int argc, char** argv) {
    enh_list enh;
    cin >> enh;
    matrix input;
    cin >> input;

    int steps = atoi(argv[1]);
    matrix output = input;
#ifdef DEBUG
    cout << "input:\n" << input << endl;
#endif
    for (int s = 0; s < steps; ++s) {
        output = enhance(input, enh);
        inf_bit_value = resolve_pixel(input, -3, -3, enh);
#ifdef DEBUG
        cout << "after step #" << s << "\n" << output << endl;
#endif
        input = output;
    }
    int non_zero_elements = 0;
    for (auto& r: output) {
        for (auto v : r) {
            if (v) ++non_zero_elements;
        }
    }
    cout << "non zero elements: " << non_zero_elements << endl;
    cout << " inf bit is: " << inf_bit_value << endl;

    return 0;
}