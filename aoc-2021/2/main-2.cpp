#include <iostream>
#include <map>
#include <functional>

int main(int argc, char** argv) {
    int h, d, aim;
    h = d = aim = 0;
    std::string cmd;
    int n;
    while (!std::cin.eof()) {
        std::cin >> cmd >> n;
        std::map<std::string, std::function<void(void)>>{
            {"forward", [&](){h += n; d += aim * n;}},
            {"down", [&](){aim += n;}},
            {"up", [&](){aim -= n;}},
        }[cmd]();
    }

    std::cout << h << " " << d << std::endl;
    std::cout << h*d << std::endl;
	return 0;
}
