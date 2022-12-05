#include "../common/filereader.h"

std::vector<std::vector<char>> parse_dumbass_format() {
    std::ifstream file("input.txt");
    std::vector<std::string> lines;
    std::string line;

    std::vector<std::vector<char>> stacks = std::vector<std::vector<char>>(0);
    while (std::getline(file, line)) {
        // Dynamically make as many stacks as you need
        if (stacks.size() < (line.length()+1)/4)
            stacks.resize((line.length()+1)/4);
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

std::vector<int> parse_instruction(std::string& instruction) {
    std::vector<std::string> data;
    std::string temp;
    std::stringstream ss(instruction);
    while (ss >> temp) data.push_back(temp);
    std::vector<int> stack_numbers;
    for (size_t i = 1; i < 6; i+=2)
        stack_numbers.push_back(std::stoi(data[i]));
    return stack_numbers;
}

int main() {

    auto stacks_p1 = parse_dumbass_format();
    auto stacks_p2 = stacks_p1;

    auto lines = aoc_read_lines_raw();
    
    // P1
    for (auto& instruction : lines) {
        if (instruction.length() == 0 || instruction[0] != 'm') continue;
        auto movement = parse_instruction(instruction);
        for (size_t i = 0; i < movement[0]; i++) {
            stacks_p1[movement[2]-1].push_back(stacks_p1[movement[1]-1].back());
            stacks_p1[movement[1]-1].pop_back();
        }
    }
    std::string message = "";
    for (auto& stack : stacks_p1) message += stack.back();
    std::cout << message << std::endl;
    
    
    // P2
    std::vector<char> temp;
    for (auto& instruction : lines) {
        if (instruction.length() == 0 || instruction[0] != 'm') continue;
        auto movement = parse_instruction(instruction);
        for (size_t i = 0; i < movement[0]; i++) {
            temp.push_back(stacks_p2[movement[1]-1].back());
            stacks_p2[movement[1]-1].pop_back();
        }
        for (size_t i = 0; i < movement[0]; i++) {
            stacks_p2[movement[2]-1].push_back(temp.back());
            temp.pop_back();
        }
    }
    message = "";
    for (auto& stack : stacks_p2) message += stack.back();
    std::cout << message << std::endl;

    return 0;
}