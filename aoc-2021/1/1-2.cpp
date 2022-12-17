#include <iostream>
#include <limits>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
	int trailing_window = std::atoi(argv[1]);
	std::vector<int> v(trailing_window, 0);
	int curr, prev;
	curr = prev = std::numeric_limits<int>::max();
	int counter = 0;
	int it = 0;
	while (!std::cin.eof()) {
		std::cin >> v[it++%trailing_window];
		curr = std::accumulate(v.begin(), v.end(), 0);
		if (it >= trailing_window) {
		       if (curr > prev) {
				++counter;
			}
			prev = curr;
		}
	}
	std::cout << counter << std::endl;
	return 0;
}

