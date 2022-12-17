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

using map_t = std::map<std::string, std::set<std::string>>;
const std::string START_NODE = "start";
const std::string END_NODE = "end";

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vals) {
    for (const auto& v : vals) {
        os << v << ",";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const map_t& m) {
    for (auto& kv : m) {
        os << kv.first << ": ";
        for (auto& v : kv.second) {
            os << v << " ";
        }
        os << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream& is, map_t& m) {
    std::string s_from, s_to;
    while (std::getline(is, s_from, '-')) {
        std::getline(is, s_to);
        m[s_from].emplace(s_to);
        m[s_to].emplace(s_from);
    }
    return is;
}

int count_paths(map_t const& m, std::string const& from, std::string const& to) {
    std::map<std::string, int> visits_left;
    for (auto it = m.begin(); it != m.end(); ++it) {
        visits_left[it->first] = std::islower(it->first[0]) ? 1 : std::numeric_limits<int>::max();
    }
    visits_left[START_NODE] = 1;
    visits_left[END_NODE] = 1;
    bool visited_twice = false;
    std::vector<std::string> curr_path;
    std::function<int(const std::string&, const std::string&)> count_paths_impl;
    count_paths_impl = [&visits_left, &visited_twice, &m, &count_paths_impl, &curr_path](std::string const& from, std::string const& to) {
        curr_path.push_back(from);
// #ifdef DEBUG
//             std::cout << from << " / " << "visited_twice=" << visited_twice << " / " << curr_path << "\n";
// #endif
        if (from == to) {
#ifdef DEBUG
            std::cout << curr_path << "\n";
#endif
            curr_path.pop_back();
            return 1;
        }

        if (visits_left[from] < 0) {
            curr_path.pop_back();
            return 0;
        }

        bool restrict_visit_once = std::islower(from[0]);
        bool reset_visit_twice = false;
        if (visits_left[from] == 0 && restrict_visit_once) {
            if (!visited_twice && from != START_NODE && from != END_NODE) {
                visited_twice = true;
                reset_visit_twice = true;
                --visits_left[from];
            } else {
                curr_path.pop_back();
                return 0;
            }
        } else {
            --visits_left[from];
        }

        int final_paths = 0;
        auto inext = m.find(from);
        if (inext != m.end()) {
            for (const auto& next : inext->second) {
                final_paths += count_paths_impl(next, to);
            }
        }

        curr_path.pop_back();
        ++visits_left[from];
        if (reset_visit_twice) visited_twice = false;
        return final_paths;
    };
    return count_paths_impl(from, to);
}

int main(int argc, char** argv) {
    map_t paths;
    std::cin >> paths;
#ifdef DEBUG
    std::cout << paths << "\n";
#endif
    int total_paths = count_paths(paths, START_NODE, END_NODE);
    std::cout << "total paths: " << total_paths << std::endl;
    return 0;
}