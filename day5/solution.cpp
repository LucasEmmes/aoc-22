#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::vector<std::vector<char>> parse_dumbass_format() {
    std::ifstream file("input.txt");
    std::vector<std::string> lines;
    std::string line;

    std::vector<std::vector<char>> stacks;
    while (std::getline(file, line)) {
        // Dynamically make as many stacks as you need
        if (stacks.size() < (line.length()+1)/4)
            stacks.reserve((line.length()+1)/4);
        // Break when we reach numbering
        if (line[1] == '1') break;
        // Otherwise, add characters wherever there isn't whitespace
        for (size_t i = 0; i < 9; i++)
            if (line[1+i*4] != ' ')
                stacks[i].push_back(line[1+i*4]);
    }

    // Reverse stacks so the top items are at the back, so they can be popped
    for (auto& stack : stacks) std::reverse(stack.begin(), stack.end());

    return stacks;
}

int main() {

    auto stacks = parse_dumbass_format();

    return 0;
}