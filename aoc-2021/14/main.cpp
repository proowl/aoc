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

using namespace std;

int main(int argc, char** argv) {
    string tmpl;
    cin >> tmpl;

    map<string, char> rules;
    set<char> alph;
    string tmp;
    while (!std::cin.eof()) {
        string pattern;
        char ch;
        cin >> pattern;
        if (cin.bad() || pattern.empty()) break;
        cin >> tmp >> ch;
        rules[pattern] = ch;
        alph.insert(pattern[0]);
        alph.insert(pattern[1]);
        alph.insert(ch);
    }

    map<string, long> pair_frequency;
    for (int i = 0; i < tmpl.size() - 1; ++i) {
        if (rules.find(tmpl.substr(i, 2)) != rules.end()) {
            pair_frequency[tmpl.substr(i, 2)] += 1;
        }
    }

#ifdef DEBUG
    for (auto it = rules.begin(); it != rules.end(); ++it) {
        cout << it->first << " " << it->second << "\n";
    }
    cout << "alphabet: " << alph.size() << " rules: " << rules.size() << " needed for full coverage:" << alph.size() * alph.size() << endl;
    for (auto it = pair_frequency.begin(); it != pair_frequency.end(); ++it) {
        cout << it->first << ": " << it->second << "\n";
    }
#endif

    int step = 0;
    int total_steps = std::atoi(argv[1]);
    while (++step <= total_steps) {
#ifdef V1
        int rit = 0;
        int wit = 1;
        while (rit < tmpl.size()-1) {
            auto it = rules.find(tmpl.substr(rit, 2));
            if (it != rules.end()) {
                tmpl.insert(wit, 1, it->second);
                ++rit;
                ++wit;
            }
            ++rit;
            ++wit;
        }
#ifdef DEBUG
        cout << "Step " << std::setw(3) << step << " : " << tmpl.substr(0, 100) << (tmpl.size() > 90 ? "...": "") << " /" << tmpl.size() << "\n";
#endif
#else
        map<string, long> next;
        for (auto it = pair_frequency.begin(); it != pair_frequency.end(); ++it) {
            string k1(2, '\0');
            char ch = rules[it->first];
            k1[0] = it->first[0];
            k1[1] = ch;
            string k2(2, '\0');
            k2[1] = it->first[1];
            k2[0] = ch;
            next[k1] += it->second;
            next[k2] += it->second;
        }
        pair_frequency = next;
#ifdef DEBUG
        cout << "Step " << std::setw(3) << step << ": ";
        for (auto it = pair_frequency.begin(); it != pair_frequency.end(); ++it) {
            cout << it->first << "(" << it->second << ") ";
        }
        cout << "\n";
#endif
#endif
    }

    long most_common = 0;
    std::map<char, long> freq;
#ifdef V1
    for (char ch : tmpl) {
        freq[ch] += 1;
        if (freq[ch] > most_common) most_common = freq[ch];
    }
#else
    freq[tmpl.front()] += 1;
    freq[tmpl.back()] += 1;

    for (auto it = pair_frequency.begin(); it != pair_frequency.end(); ++it) {
        freq[it->first[0]] += it->second;
        freq[it->first[1]] += it->second;
    }
#endif
    long least_common = numeric_limits<long>::max();
    for (auto it = freq.begin(); it != freq.end(); ++it) {
        if (it->second > most_common) most_common = it->second;
        if (it->second < least_common) least_common = it->second;
    }
    most_common /= 2;
    least_common /= 2;
    cout << "length: " << tmpl.size() << endl;
    cout << "result p1: " << (most_common - least_common) << " : " << most_common << " - " << least_common << endl;
    return 0;
}