#include <iostream>
#include <vector>
#include <array>
#include <functional>
#include <sstream>
#include <algorithm>
#include <numeric>

// #define DEBUG
// #define FIRST_WIN // 25023

constexpr int size = 5;
using matrix = std::vector<std::vector<int>>;

std::vector<int> read_row(const std::string& s, char delim) {
    std::stringstream in(s);
    std::vector<int> out;
    std::string sv;
    while (in.good()) {
        std::getline(in,sv,delim);
        if (sv.empty()) continue;
        out.push_back(std::stoi(sv));
    }
    return out;
}

void print_row(std::vector<int> const& vals) {
    for (auto& v : vals) {
        std::cout.width(3);
        std::cout << v << " ";
    }
    std::cout << "\n";
}

int main(int argc, char** argv) {
    std::vector<matrix> cards;
    cards.emplace_back();
    std::vector<int> input;
    std::string s;


    while (!std::cin.eof()) {
        std::getline(std::cin, s);
#ifdef DEBUG
        std::cout << "read " << s << std::endl;
#endif
        if (!s.empty()) {
            if (input.empty()) {
#ifdef DEBUG
                std::cout << "read input" << std::endl;
#endif
                input = read_row(s, ',');
            } else {
#ifdef DEBUG
                std::cout << "read card" << std::endl;
#endif
                matrix& m = [](std::vector<matrix>& cards) -> matrix& {
                    if (cards.back().size() >= size) {
                        cards.emplace_back();
                    }
                    return cards.back();
                }(cards);
                m.emplace_back();
                m.back() = read_row(s, ' ');
            }
        }
    }

    matrix winner;
    int winner_n = 0;
    std::vector<bool> already_won(cards.size(), false);
    for (int n : input) {
        for (int ic = 0; ic < cards.size(); ++ic) {
            auto& c = cards[ic];
            // std::vector<bool> hbingo(c.size(), true);
            // std::vector<bool> vbingo(c.size(), true);
            std::vector<bool> hbingo(true, c.size());
            std::vector<bool> vbingo(true, c.size());
            for (int i = 0; i < c.size(); ++i) {
                for (int j = 0; j < c.size(); ++j) {
                    if (c[i][j] == n)  {
                        c[i][j] = -1;
                    }
                    bool crossed = c[i][j] == -1;
                    vbingo[j] = vbingo[j] && crossed;
                    hbingo[i] = hbingo[i] && crossed;
                }
            }
            bool is_bingo = 
                (std::find(hbingo.begin(), hbingo.end(), true) != hbingo.end())
                ||
                (std::find(vbingo.begin(), vbingo.end(), true) != vbingo.end());
            if (is_bingo && !already_won[ic]) {
                winner = c;
                winner_n = n;
                already_won[ic] = true;
#ifdef FIRST_WIN
                break;
#endif
            }
        }
#ifdef FIRST_WIN
        if (winner.size() > 0) break;
#endif
    }

    int sum = 0;
    for (auto const& r: winner) {
        for (int v : r) {
            if (v > 0) sum += v;
        }
    }

#ifdef DEBUG
    std::cout << "Input: \n";
    print_row(input);
    std::cout << "Data: " << cards.size() << "\n";
    for (auto& c : cards) {
        for(auto& r : c) {
            print_row(r);
        }
        std::cout << "\n";
    }
#endif
    std::cout << "Winner: \n";
    for(auto& r : winner) {
        print_row(r);
    }
    std::cout << "\n";
    std::cout << "sum=" << sum << "\n"
        << "n=" << winner_n << "\n"
        << "result=" << sum * winner_n << "\n";
    std::cout << std::endl;
	return 0;
}
