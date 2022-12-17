
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
#include <thread>
#include <atomic>
#include <regex>

using namespace std;

using vars_t = unordered_map<char, long>;
using commands_t = vector<tuple<string, char, string>>;

const set<char> variables = {'x','y','z'};

atomic<long> processed_count = 0;
atomic<long> answer = 0;

long get_val(vars_t const& vars, const string& arg) {
    if (arg.empty()) return 0;
    if (arg.size() > 1 || variables.count(arg[0]) == 0) {
        return stoi(arg);
    }
    return vars.find(arg[0])->second;
}

// void check_numbers(commands_t const& commands, int from, int to) {
//     for (long i = to; i >= from; --i) {
//         string tmp = to_string(i);
//         ++processed_count;
//         if (tmp.find_first_of('0') < tmp.size()) continue;

//         vars_t vars;
//         int input_pos = 0;
//         for (char v : variables) vars[v] = 0;
//         for (auto [cmd, var, arg] : commands) {
//             long val = get_val(vars, arg);
//             if (cmd == "inp") {
//                 vars[var] = tmp[input_pos++] - '0';
//             } else if (cmd == "add") {
//                 vars[var] += val;
//             } else if (cmd == "mul") {
//                 vars[var] *= val;
//             } else if (cmd == "div") {
//                 vars[var] /= val;
//             } else if (cmd == "mod") {
//                 vars[var] = vars[var] % val;
//             } else if (cmd == "eql") {
//                 vars[var] = vars[var] == val ? 1 : 0;
//             }
//         }
//         if (vars['z'] == 0) {
//             cout << i << " STONKS" << endl;
//             break;
//         }
//     }
// }

bool is_var(string val) {
    return val.size() == 1 && variables.count(val[0]) > 0;
}

string normalize_arg(map<string, string> formulas, int wid, string arg) {
    if (arg.empty())
        return arg;
    if (is_var(arg)) {
        return formulas[arg];
    } else if (arg == "w") {
        return string("w[") + to_string(wid) + "]";
    }
    return arg;
}

string maybe_wrap(string formula) {
    if (formula.find_first_of(" ") < formula.size() || formula.front() == '-') {
        return "(" + formula + ")";
    }
    return formula;
}

tuple<bool, long> parse_number(string arg) {
    if (arg.empty()) {
        return make_tuple(true, 0);
    }
    std::size_t pos;
    try {
        long parsed = stol(arg, &pos);
        return make_tuple(pos==arg.size(), parsed);
    } catch(...) {
        return make_tuple(false, 0);
    }
}

bool is_w(string arg) {
    static const regex re("w\\[\\d+\\]");
    cmatch m;
    return regex_match(arg.c_str(), m, re);
}

void optimize_input() {
    string s;
    commands_t commands;
    map<string, string> formulas;
    int w_id = -1;
    int cmd_count = 0;
    while(getline(cin, s)) {
        ++cmd_count;

        string cmd = s.substr(0,3);
        string var;
        var.push_back(s[4]);
        string arg = s.size() >= 6 ? s.substr(6) : "";
        arg = normalize_arg(formulas, w_id, arg);
        if (cmd == "inp") {
            w_id += 1;
        } if (cmd == "mul") {
            if (formulas[var].empty() || arg.empty() || arg == "0" || formulas[var] == "0") {
                // multiply by 0 -> nothing
                formulas[var] = "";
            } else {
                auto [is_left_number, left] = parse_number(formulas[var]);
                auto [is_right_number, right] = parse_number(arg);
                if (is_left_number && is_right_number) {
                    formulas[var] = to_string(left * right);
                } else {
                    if (is_right_number && right == 1) {
                        // do nothing
                    } else {
                        formulas[var] = maybe_wrap(formulas[var]) + " * " + maybe_wrap(arg);
                    }
                }
            }
        } else if (cmd == "add") {
            if (cmd_count == 78) {
                // 78: w[3] + 10 - 12
                formulas[var] = "w[3] - 2";
            } else if (arg.empty() || arg == "0") {
                // do nothing
            } else if (formulas[var].empty()) {
                formulas[var] = arg;
            } else {
                auto [is_left_number, left] = parse_number(formulas[var]);
                auto [is_right_number, right] = parse_number(arg);
                if (is_left_number && is_right_number) {
                    formulas[var] = to_string(left + right);
                } else {
                    if (is_right_number && right < 0) {
                        formulas[var] = formulas[var] + " - " + to_string(-right);
                    } else {
                        formulas[var] = formulas[var] + " + " + maybe_wrap(arg);
                    }
                }
            }
        } else if (cmd == "div") {
            if (arg.empty() || arg == "0") {
                cerr << "invalid command: division by 0! >> " << s << endl;
                exit(1); 
            }
            if (cmd_count == 77) {
                // 77: ((((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)) * 26 + (w[3] + 10)) / 26
                //  -> ((w[0] + 8) * 26 + w[1] + 8) * 26 + w[2] + 3
                formulas[var] = "((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)";
            } else if (cmd_count == 113) {
                // 113: ((((w[0] + 8) * 26 + w[1] + 8) * 26 + w[2] + 3) * 26 + (w[5] + 8)) / 26
                formulas[var] = "((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)";
            } else if (cmd_count == 131) {
                // 131: (((w[0] + 8) * 26 + w[1] + 8) * 26 + w[2] + 3) / 26
                formulas[var] = "(w[0] + 8) * 26 + (w[1] + 8)";
            } else if (cmd_count == 185) {
                // 185: ((((w[0] + 8) * 26 + w[1] + 8) * 26 + (w[8] + 9)) * 26 + (w[9] + 3)) / 26
                formulas[var] = "((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[8] + 9)";
            } else if (cmd_count == 203) {
                // 203: (((w[0] + 8) * 26 + w[1] + 8) * 26 + w[8] + 9) / 26
                formulas[var] = "(w[0] + 8) * 26 + (w[1] + 8)";
            } else if (cmd_count == 221) {
                // 221: (((w[0] + 8) * 26 + w[1] + 8) / 26
                formulas[var] = "w[0] + 8";
            } else if (cmd_count == 239) {
                // 239: (w[0] + 8) / 26
                formulas[var] = "";
            } else if (formulas[var].empty()) {
                // do nothing: 0/arg=0
            } else if (arg == "1") {
                // do nothing: x/1 = x
            } else {
                auto [is_left_number, left] = parse_number(formulas[var]);
                auto [is_right_number, right] = parse_number(arg);
                if (is_left_number && is_right_number) {
                    formulas[var] = to_string(left / right);
                } else {
                    formulas[var] = maybe_wrap(formulas[var]) + " / " + arg;
                }
            }
        } else if (cmd == "mod") {
            static const map<string, string> optimisations = {
                {"(w[0] + 8) % 26", "w[0] + 8"},
                {"((w[0] + 8) * 26 + (w[1] + 8)) % 26", "w[1] + 8"},
                {"(((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)) % 26", "w[2] + 3"},
                {"((((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)) * 26 + (w[3] + 10)) % 26", "w[3] + 10"},
                {"((((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[2] + 3)) * 26 + (w[5] + 8)) % 26", "w[5] + 8"},
                {"(((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[8] + 9)) % 26", "w[8] + 9"},
                {"((((w[0] + 8) * 26 + (w[1] + 8)) * 26 + (w[8] + 9)) * 26 + (w[9] + 3)) % 26", "w[9] + 3"},
            };
            if (arg.empty() || arg == "0") {
                cerr << "invalid command: division by 0! >> " << s << endl;
                exit(1); 
            } else if (formulas[var].empty()) {
                // do nothing: 0%arg=0
            } else {
                auto [is_left_number, left] = parse_number(formulas[var]);
                auto [is_right_number, right] = parse_number(arg);
                if (is_left_number && is_right_number) {
                    formulas[var] = to_string(left % right);
                } else {
                    formulas[var] = maybe_wrap(formulas[var]) + " % " + arg;
                }
            }
            auto opt = optimisations.find(formulas[var]);
            if (opt != optimisations.end()) {
                formulas[var] = opt->second;
            }
        } else if (cmd == "eql") {
            auto [is_left_number, left] = parse_number(formulas[var]);
            auto [is_right_number, right] = parse_number(arg);

            static const set always_false = {25, 43, 61, 80, 97, 116, 134, 151, 169, 188, 206, 224, 242};
            if (always_false.count(cmd_count) > 0) {
                // 25: (w[0] + 8 + 13) == w[1]; (w[0] + 8 + 13) > w[1] -> false
                // 43: (w[1] + 8 + 13) == w[2]
                // 61: (w[2] + 3 + 12) == w[3]
                // 80: ((w[3] - 2) == w[4]) == 0 -> w[3] -2 = w[4]
                // 97: (w[2] + 3 + 12) == w[5]
                // 116: ((w[5] + 8 - 2) == w[6]) == 0 -> w[5] + 6 = w[6] 
                // 134: ((w[2] + 3 - 11) == w[7]) == 0 -> w[2] - 8 = w[7] ???
                // 151: (w[1] + 8 + 13) == w[8]
                // 169: (w[8] + 9 + 14) == w[9]
                // 188: ((w[9] + 3) == w[10]) == 0 -> w[9] + 3 = w[10]
                // 206: ((w[8] + 9 - 12) == w[11]) == 0 -> w[8] - 3 = w[11]
                // 224: ((w[1] + 8 - 13) == w[12]) == 0 -> w[1] - 5 = w[12]
                // 242: ((w[0] + 8 - 6) == w[13]) == 0 -> w[0] + 2 = w[13]
                formulas[var] = "";
            } else if (is_left_number && is_right_number && left == right) {
               formulas[var] = "1";
            } else if (is_left_number && (left < 0 || left > 9) && is_w(arg)) {
                formulas[var] = "";
            } else {
                formulas[var] = maybe_wrap(formulas[var]) + " == " + maybe_wrap(arg);
            }
        }

        cout << ":" << cmd_count << " ";
        for (auto& kv : formulas) {
            cout << kv.first << "='" << kv.second << "' ";
        }
        // cout << "z='" << formulas["z"] << "' ";
        cout << "w_id=" << w_id << endl;
// #define ITERATIONS 100
        // if (cmd_count >= ITERATIONS) break;
    }
}

long z_formula(const vector<long>& w) {
    return 0;
}

atomic<int> finished_count = 0;
void check_numbers(long from, long to) {
    for (long i = to; i >= from; --i) {
        long var = i;
        vector<long> w;
        while (var > 0) {
            long add = var % 10;
            if (add == 0) break;
            w.insert(w.begin(), add);
            var = var / 10;
        }
        ++processed_count;
        if (w.size() < 14) continue;
        if (w[3] - 2 != w[4]) continue;
        if (w[5] + 6 != w[6]) continue;
        if (w[2] - 8 != w[7]) continue;
        if (w[9] + 3 != w[10]) continue;
        if (w[8] - 3 != w[11]) continue;
        if (w[1] - 5 != w[12]) continue;
        if (w[0] + 2 != w[13]) continue;

        long z = z_formula(w);
        if (z == 0) {
            if (i < answer || answer == 0) answer = i;
            // break;
        }
    }
    finished_count += 1;
}

void simulate() {
    long thread_count = 16;
    //             12345678901234
    // long i_start = 79990000000000;
    // long i_end   = 79999999999999;
    long i_start = 16931170000000;
    long i_end   = 16931179999999;
    long step = ceil(i_end-i_start)/thread_count;

    thread watcher([=](){
        long slept = 0;
        while (finished_count < thread_count) {
            std::this_thread::sleep_for(10s);
            slept += 10;
            cout << "processed_count=" << setw(3) << (100.0*processed_count/(i_end - i_start))
                << "% processed/sec=" << (processed_count/slept)
                << " est_answer=" << answer << endl;
        }
        cout << "final answer: " << answer << endl;
    });

    vector<thread> threads;
    for (long i = i_start; i < i_end; i += step + 1) {
        threads.push_back(thread(check_numbers, i, i+step));
    }
    for (auto& t : threads) {
        t.join();
    }
    watcher.join();
}

int main(int argc, char** argv) {
#ifdef OPT_INPUT
    optimize_input();
#else
    simulate();
#endif
    return 0;
}
