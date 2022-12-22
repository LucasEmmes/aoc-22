#include "../common/filereader.h"

#include <unordered_map>
#include <memory>
#include <functional>

struct Monkey {
    int64_t value;
    std::string a;
    std::string b;
    std::function<int64_t(int64_t, int64_t)> operation;
    char opchar;
    bool human;
};

std::unordered_map<std::string, std::shared_ptr<Monkey>> monkies;

void make_monkies(std::vector<std::vector<std::string>>& lines) {
    for (auto& line : lines) {
        auto m = std::make_shared<Monkey>();
        monkies[line[0]] = m;
        m->human = (line[0] == "humn");
        // Number monkey
        if (line.size() == 2) {
            m->value = std::stoll(line[1]);
        }
        // Operation monkey
        else {
            m->value = INT64_MAX;
            m->a = line[1];
            m->b = line[3];
            if (line[2][0] == '+') m->operation = std::plus<int64_t>();
            else if (line[2][0] == '-') m->operation = std::minus<int64_t>();
            else if (line[2][0] == '*') m->operation = std::multiplies<int64_t>();
            else if (line[2][0] == '/') m->operation = std::divides<int64_t>();
            m->opchar = line[2][0];
        }
    }
}

int64_t get_monkey_value(std::string& name) {
    auto monkey = monkies[name];
    
    // If monkey has been calculated
    if (monkey->value != INT64_MAX) return monkey->value;

    // If operation monkey and not calculated yet
    auto value_a = get_monkey_value(monkey->a);
    auto value_b = get_monkey_value(monkey->b);
    monkey->value = monkey->operation(value_a, value_b);
    return monkey->value;
}

bool contains_human(std::string& name) {
    auto monkey = monkies[name];
    
    // If reached bottom, return answer
    if (monkey->human) return true;
    if (monkey->value != INT64_MAX) return false;

    // Check each part and return
    return contains_human(monkey->a) || contains_human(monkey->b);
}

void rec(std::string name, int64_t expected_value) {
    // Base case
    if (name == "humn") {
        std::cout << expected_value << std::endl;
        return;
    }

    auto monkey = monkies[name];

    // Left side contains human
    if (contains_human(monkey->a)) {
        auto right = get_monkey_value(monkey->b);
        int64_t expected_left;
        if (monkey->opchar == '+') expected_left = expected_value - right;
        else if (monkey->opchar == '-') expected_left = expected_value + right;
        else if (monkey->opchar == '*') expected_left = expected_value / right;
        else if (monkey->opchar == '/') expected_left = expected_value * right;
        rec(monkey->a, expected_left);
    }
    // Right side contains human
    else {
        auto left = get_monkey_value(monkey->a);
        int64_t expected_right;
        if (monkey->opchar == '+') expected_right = expected_value - left;
        if (monkey->opchar == '-') expected_right = left - expected_value;
        if (monkey->opchar == '*') expected_right = expected_value / left;
        if (monkey->opchar == '/') expected_right = left / expected_value;
        rec(monkey->b, expected_right);
    }
    return;
}

int main() {
    auto lines = aoc_read_input_lines_split({':'});

    make_monkies(lines);

    // P1
    std::string rootref = "root";
    std::cout << get_monkey_value(rootref) << "\n" << std::endl;

    // P2
    monkies.clear();
    make_monkies(lines);

    auto left = monkies[rootref]->a;
    auto right = monkies[rootref]->b;

    if (contains_human(left)) {
        rec(left, get_monkey_value(right));
    } else {
        rec(right, get_monkey_value(left));
    }
}