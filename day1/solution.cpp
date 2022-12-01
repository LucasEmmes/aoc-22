#include <algorithm>

#include "../common/filereader.h"

int main() {

    auto lines = aoc_read_lines_raw();

    // P1
    std::vector<size_t> elves;
    size_t calorie_count = 0;
    for (auto& calories : lines) {
        if (calories.length() > 0) {
            calorie_count += std::stol(calories);
        } else {
            elves.push_back(calorie_count);
            calorie_count = 0;
        }
    }
    std::sort(elves.begin(), elves.end(), std::greater<size_t>());
    std::cout << elves[0] << std::endl;

    // P2
    std::cout << (elves[0] + elves[1] + elves[2]) << std::endl;
    
    return 0;
}