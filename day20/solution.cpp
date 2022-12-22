#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>

#include <cmath>
#include <unordered_map>

std::vector<std::string> aoc_read_lines_raw() {
    std::ifstream file("../input.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        if (line.size())
            lines.push_back(line);
    return lines;
}

struct Link {
    std::shared_ptr<Link> previous;
    int64_t value;
    std::shared_ptr<Link> next;
};

int64_t instruction_size;

std::shared_ptr<Link> get_link_number(std::shared_ptr<Link> link, int64_t number) {
    if (number >= instruction_size-1)
        number %= (instruction_size-1);
    else if (number <= -1*(instruction_size-1))
        number = -1*(std::abs(number)%(instruction_size-1));
    
    if (number > 0)
        for (int64_t i = 0; i < number; i++)
            link = link->next;
    else if (number < 0)
        for (int64_t i = 0; i > number; i--)
            link = link->previous;
    return link;
}

int main(int argc, char** argv) {
    auto lines = aoc_read_lines_raw();
    // std::vector<std::string> lines = {"0","1","1","8"};

    std::vector<std::shared_ptr<Link>> instructions;
    std::shared_ptr<Link> zero;

    for (size_t i = 0; i < lines.size(); i++) {
        int64_t value = std::stoll(lines[i]) * 811589153;
        auto link = std::make_shared<Link>();
        link->value = value;

        // Link this and previous element (if there is one)
        if (i > 0) {
            instructions.back()->next = link;
            link->previous = instructions.back();
        }

        // Get the zero-link
        if (value == 0) zero = link;
        instructions.push_back(link);
    }

    instruction_size = instructions.size();

    // Link first and last element
    instructions.front()->previous = instructions.back();
    instructions.back()->next = instructions.front();

    std::shared_ptr<Link> old_next, old_previous, new_next, new_previous;

    // Main loop
    for (size_t i = 0; i < 10; i++) {
        for (auto& link : instructions) {
            if (link->value == 0) continue;

            auto p = get_link_number(link, link->value);
            if (link == p) continue;

            old_next = link->next;
            old_previous = link->previous;

            if (link->value > 0) {
                // Note down links that need changing
                new_next = p->next;
                new_previous = p;
            } else {
                // Note down links that need changing
                new_next = p;
                new_previous = p->previous;
            }
            // Fill the hole
            old_previous->next = old_next;
            old_next->previous = old_previous;

            // Insert self
            new_next->previous = link;
            new_previous->next = link;
            link->next = new_next;
            link->previous = new_previous;
        }
    }
    // auto l = instructions.front();
    // std::cout << l->value << " ";
    // l = l->next;
    // while (l != instructions.front()) {
    //     std::cout << l->value << " ";
    //     l = l->next;
    // }
    // std::cout  << std::endl;


    int64_t a, b, c;
    auto it = zero;
    for (size_t i = 1; i <= 3000; i++) {
        it = it->next;
        if (i==1000) a = it->value;
        if (i==2000) b = it->value;
        if (i==3000) c = it->value;
    }

    std::cout << a << " + " << b << " + " << c << " = " << a+b+c << std::endl;

    return 0;
}
