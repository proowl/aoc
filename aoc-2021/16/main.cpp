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

using namespace std;

struct Parser {
    enum State {
        UNKNOWN,
        VERSION,
        TYPE_ID,
        LITERAL,
        OP_LEN_TYPE_ID,
        TOTAL_LEN,
        TOTAL_SUB_PACKETS,
        SUBPACKETS_BITS,
        SUBPACKETS_COUNT,
    };

    map<State, int> min_width = {
        {VERSION, 3},
        {TYPE_ID, 3},
        {LITERAL, 5},
        {UNKNOWN, 0},
        {OP_LEN_TYPE_ID, 1},
        {TOTAL_LEN, 15},
        {TOTAL_SUB_PACKETS, 11},
        {SUBPACKETS_BITS, 0},
        {SUBPACKETS_COUNT, 0},
    };

    map<State, const char*> name = {
        {VERSION, "VERSION"},
        {TYPE_ID, "TYPE_ID"},
        {LITERAL, "LITERAL"},
        {UNKNOWN, "-"},
        {OP_LEN_TYPE_ID, "OP_LEN_TYPE_ID"},
        {TOTAL_LEN, "TOTAL_LEN"},
        {TOTAL_SUB_PACKETS, "TOTAL_SUB_PACKETS"},
        {SUBPACKETS_BITS, "SUBPACKETS_BITS"},
        {SUBPACKETS_COUNT, "SUBPACKETS_COUNT"},
    };

    map<State, State> next_state = {
        {VERSION, TYPE_ID},
    };

    deque<bool> buf;
    long bit_offset = 0;
    long packet_offset = 0;
    // state, offset, consume_count
    stack<tuple<State, long, long>> cmd;

    bool try_consume(
        function<void(long)>& version_visitor,
        function<void(long)>& op_visitor,
        function<void(long)>& literal_visitor,
        function<void(bool)>& packet) {

        auto [state, offset, consume_count] = cmd.top();
        if (buf.size() < min_width[state]) return !cmd.empty();
        cmd.pop();

        for (int i = 0; i < cmd.size(); ++i) cout << " ";
        cout << ": " << name[state] << " (" << offset << " " << consume_count  << ") cur(" << bit_offset << ", " << packet_offset << ")\n";
        for (int i = 0; i < cmd.size(); ++i) cout << " ";
        long consumed_val = consume_to_int(state, min_width[state]);
        cout << " val: " << consumed_val << "\n";

        State next = UNKNOWN;
        switch (state) {
            case VERSION:
                next = next_state[state];
                cmd.push({next, 0, 0});
                version_visitor(consumed_val);
                break;
            case TYPE_ID:
                if (consumed_val == 4) {
                    next = LITERAL;
                } else {
                    next = OP_LEN_TYPE_ID;
                    packet(/*start=*/true);
                    op_visitor(consumed_val);
                }
                cmd.push({next, 0, 0});
                break;
            case OP_LEN_TYPE_ID:
                if (consumed_val == 0) {
                    next = TOTAL_LEN;
                } else {
                    next = TOTAL_SUB_PACKETS;
                }
                cmd.push({next, 0, 0});
                break;
            case LITERAL:
                next = UNKNOWN;
                ++packet_offset;
                literal_visitor(consumed_val);
                break;
            case TOTAL_LEN:
                next = SUBPACKETS_BITS;
                ++packet_offset;
                cmd.push({next, bit_offset, consumed_val});
                break;
            case TOTAL_SUB_PACKETS:
                next = SUBPACKETS_COUNT;
                ++packet_offset;
                cmd.push({next, packet_offset, consumed_val});
                break;
            case SUBPACKETS_BITS:
                if (offset + consume_count > bit_offset) {
                    cmd.push({SUBPACKETS_BITS, offset, consume_count});
                    cmd.push({VERSION, 0, 0});
                    next = VERSION;
                } else {
                    packet(/*start=*/false);
                }
                break;
            case SUBPACKETS_COUNT:
                if (consume_count > 0) {
                    cmd.push({SUBPACKETS_COUNT, offset, consume_count-1});
                    cmd.push({VERSION, 0, 0});
                    next = VERSION;
                } else {
                    packet(/*start=*/false);
                }
                break;
            case UNKNOWN:
            default:
                return !cmd.empty();
        }
        return !cmd.empty();
    }

    long consume_to_int(State state, int width) {
        int i = 0;
        long val = 0;
        while (++i <= width) {
            if (state == LITERAL && i % 5 == 1) {
                bool end_reached = !buf.front();
                if (!end_reached) width += 5;
            } else {
                val = val << 1;
                val = val | (buf.front() ? 1 : 0);
            }

            cout << buf.front();
            buf.pop_front();
            ++bit_offset;
        }
        cout << " ";
        return val;
    }
};

deque<bool> convert(char ch) {
    if (ch >= 'A') {
        switch (ch) {
            case 'A': return {!!1, !!0, !!1, !!0};
            case 'B': return {!!1, !!0, !!1, !!1};
            case 'C': return {!!1, !!1, !!0, !!0};
            case 'D': return {!!1, !!1, !!0, !!1};
            case 'E': return {!!1, !!1, !!1, !!0};
            case 'F': return {!!1, !!1, !!1, !!1};
        }
    }

    ch = ch - '0';
    return {!!(ch & 0b1000), !!(ch & 0b0100), !!(ch & 0b0010), !!(ch & 0b0001)};
}

ostream& operator<<(ostream& os, deque<bool> const& r) {
    for (auto v : r) {
        os << (v ? '1' : '0');
    }
    os << "\n";
    return os;
}

long consume_packet(stack<tuple<char, long>>& tokens) {
    bool packet_edge_reached = false;
    auto [type, val] = tokens.top();
    tokens.pop();
    bool find_end = true;
    if (type != '(') {
        find_end = false;
    }

    vector<long> vals;
    int operation = -1;
    bool end_found = false;
    while ((!end_found && find_end) || !tokens.empty()) {
        auto [type, val] = tokens.top();
        if (type == ')') {
            end_found = true;
            tokens.pop();
            break;
        } else if (type == '(') {
            vals.push_back(consume_packet(tokens));
        } else if (type == 'l') {
            vals.push_back(val);
            tokens.pop();
        } else if (type == 'o') {
            operation = val;
            tokens.pop();
        }
    }

    // if (operation < 0 || vals.size() < 2) {
    //     cerr << "BROKEN PACKET " << "op=" << operation << " vals=" << vals.size() << endl;
    //     exit(3);
    // }

    switch (operation) {
        case 0 /*sum*/:
            return accumulate(vals.begin(), vals.end(), 0l);
        case 1 /*product*/:
            return accumulate(vals.begin(), vals.end(), 1l, multiplies<long>());
        case 2 /*min*/:
            return accumulate(vals.begin(), vals.end(), vals[0],
                [](long a, long b) { return a < b ? a : b; });
            break;
        case 3 /*max*/:
            return accumulate(vals.begin(), vals.end(), vals[0],
                [](long a, long b) { return a > b ? a : b; });
        case 5 /*greater than*/:
            return vals[0] < vals[1] ? 1 : 0;
        case 6 /*less than*/:
            return vals[0] > vals[1] ? 1 : 0;
        case 7 /*equal to*/:
            return vals[0] == vals[1] ? 1 : 0;
    }

    return val;
}

int main(int argc, char** argv) {
    char ch;
    string s;
    if (!getline(std::cin, s)) {
        return 1;
    }
    Parser p;
    for (char ch : s) {
        auto part = convert(ch);
        p.buf.insert(p.buf.end(), part.begin(), part.end());
    }
    cout << "parsed: " << p.buf << "\n";
    p.cmd.push(make_tuple(Parser::VERSION, 0, 0));
    int version_sum = 0;
    stack<tuple<char, long>> tokens;
    function<void(long)> version_acc = [&version_sum] (int version) { version_sum += version; };
    function<void(long)> op = [&] (long op) { tokens.push(make_tuple('o', op)); };
    function<void(long)> literal = [&] (long val) { tokens.push(make_tuple('l', val)); };
    function<void(bool)> packet = [&] (bool start) { tokens.push(make_tuple(start ? ')' : '(', 0)); };

    while (p.try_consume(version_acc, op, literal, packet));

    auto copy = tokens;
    map<long, char> operations = {
        {0, '+'},
        {1, '*'},
        {2, 'm'},
        {3, 'M'},
        {5, '>'},
        {6, '<'},
        {7, '='},
    };
    while (copy.size() > 0) {
        auto [type, val] = copy.top();
        if (type == 'l') cout << val << " ";
        else if (type == 'o') cout << operations[val] << " ";
        else cout << type;
        copy.pop();
    }

    cout << "\n version sum: " << version_sum << endl;
    cout << "evaluated value: " << consume_packet(tokens) << endl;
    return 0;
}