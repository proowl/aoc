#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <numeric>

int main(int argc, char** argv) {
    const int sim_days = std::atoi(argv[1]);
    std::array<long,9> vals;
    vals.fill(0);
    while (!std::cin.eof()) {
        std::string s;
        std::getline(std::cin, s, ',');
        vals[std::stoi(s)] += 1;
    }

    int day = sim_days + 1;
    while (--day) {
        long new_fish = vals[0];
        for (int i = 1; i < vals.size(); ++i) {
            vals[i-1] = vals[i];
            vals[i] = 0;
        }
        vals[8] = new_fish;
        vals[6] += new_fish;
    }
    long total = std::accumulate(vals.begin(), vals.end(), 0UL);
    std::cout << "sim days: " << sim_days << " total fish: " << total << std::endl;
    return 0;
}