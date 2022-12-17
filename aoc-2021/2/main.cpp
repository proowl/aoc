#include <iostream>
#include <map>
#include <functional>

int main(int argc, char** argv) {
    int h, d;
    h = d = 0;
    std::string cmd;
    int n;
    std::map<std::string, std::function<void(void)>> apply = {
        {"forward", [&](){h += n;}},
        {"down", [&](){d += n;}},
        {"up", [&](){d -= n;}},
    };
    while (!std::cin.eof()) {
        std::cin >> cmd >> n;
        if (!std::cin.bad()) {
            apply[cmd]();
        }
    }

    std::cout << h << " " << d << std::endl;
    std::cout << h*d << std::endl;
	return 0;
}
