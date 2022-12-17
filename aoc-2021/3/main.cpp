#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <functional>
#include <bitset>

#define BITS 5

template<typename T>
void print_any(T const& collection) {
    for(const auto& t : collection) {
        std::cout << std::bitset<BITS>(t) << " ";
    }
    std::cout << std::endl;
}

int find_last(std::vector<int> all_v, int len, bool most_common) {
    int filter_pos = len;
    while (all_v.size() > 1 && filter_pos > 0) {
        // print_any(all_v);
        int n = 0;
        int mask = 1 << (filter_pos-1);
        for (int v : all_v) {
            if (v & mask) ++n;
        }
        bool filter_ones = (n >= all_v.size()-n) && most_common || (n < all_v.size()-n) && !most_common;
        int w = 0;
        // std::cout << "filter_pos=" << filter_pos
        //     << " mask=" << std::bitset<BITS>(mask)
        //     << " filter_ones=" << filter_ones << "\n";
        for (int r = 0; r < all_v.size(); ++r) {
            if (!!(all_v[r] & mask) == filter_ones) {
                all_v[w++] = all_v[r];
            }
        }
        all_v.resize(w);
        filter_pos--;
    }
    return (all_v.size() > 0) ? all_v[0] : 0;
}

int main(int argc, char** argv) {
    int n,v;
    n = v = 0;
    std::string s;
    int gamma = 0, epsilon = 0;
    std::array<int, 12> fr;
    fr.fill(0);
    int len = 0;
    std::vector<int> all_v;
    while (!std::cin.eof()) {
        std::cin >> s;
        ++n;
        if (len == 0) len = s.size();
        int input = 0;
        for (int i = 0; i < s.size(); ++i) {
            fr[i] += (s[i] == '1');
            input = (input << 1) + (s[i] == '1');
        }
        all_v.push_back(input);
    }

    for (int i = 0; i < len; ++i) {
        auto f = fr[i];
        gamma = (gamma << 1) + (f >= n/2); 
        epsilon = (epsilon << 1) + (f < n/2);
    }

    // part 2
    int ox = find_last(all_v, len, true);
    int co2 = find_last(all_v, len, false);

    std::cout << n << std::endl;
    std::cout << "gamma=" << gamma << " " << std::bitset<BITS>(gamma) << std::endl;
    std::cout << "epsilon=" << epsilon << " " << std::bitset<BITS>(epsilon) << std::endl;
    std::cout << gamma*epsilon << std::endl;

    std::cout << "ox=" << ox << " " << std::bitset<BITS>(ox) << std::endl;
    std::cout << "co2=" << co2 << " " << std::bitset<BITS>(co2) << std::endl;
    std::cout << ox * co2 << std::endl;
    return 0;
}