#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <algorithm>
#include <vector>

int main(int argc, char** argv) {
    std::map<char, int> ch_score = {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137},
    };
    std::map<char, char> opening = {
        {')', '('},
        {']', '['},
        {'}', '{'},
        {'>', '<'},
    };
    std::map<char, int> compl_score = {
        {'(', 1},
        {'[', 2},
        {'{', 3},
        {'<', 4},
    };
    long score = 0;
    std::vector<long> completion_scores;
    while (std::cin.good()) {
        std::string s;
        std::getline(std::cin, s);
        std::stack<char> seen;
        bool is_valid = true;
        for (char c : s) {
            if (c == ')' || c == ']' || c ==  '}' || c ==  '>') {
                if (seen.top() == opening[c]) {
                    seen.pop();
                    continue;
                }
                score += ch_score[c];
                is_valid = false;
                break;
            }
            seen.push(c);
        }
        long cscore = 0;
        if (is_valid && !seen.empty()) {
            while (!seen.empty()) {
                cscore = cscore * 5 + compl_score[seen.top()];
                seen.pop();
            }
            completion_scores.push_back(cscore);
        }
    }
    std::sort(completion_scores.begin(), completion_scores.end());
    std::cout << "score: " << score << std::endl;
    std::cout << "median completion score: " << completion_scores[completion_scores.size()/2] << std::endl;
    return 0;
}