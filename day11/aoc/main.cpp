#include "../../common/filereader.h"

#include <functional>
#include <memory>

void split_line(std::string& line, std::vector<std::string>& collector) {
    collector.clear();
    std::stringstream ss(line);
    std::string temp_str;
    while (ss >> temp_str) collector.push_back(temp_str);
}

class Monkey{
public:
    uint64_t business = 0;
    std::vector<uint64_t> items;
    std::function<uint64_t(uint64_t)> op_func;
    std::function<uint64_t(uint64_t)> test_func;

    void perform_turn(std::vector<std::shared_ptr<Monkey>>& monkies_p1) {
        while (items.size()) {
            uint64_t item = items.back();
            // Perform operation
            item = op_func(item);
            // Calm down
            item /= 3;
            // Throw to next monkey
            uint64_t next_monkey = test_func(item);
            monkies_p1[next_monkey]->items.push_back(item);
            items.pop_back();
            business++;
        }
    }
};

int main() {

    auto lines = aoc_read_lines_raw();
    std::vector<std::shared_ptr<Monkey>> monkies_p1;

    std::vector<std::string> line;
    for (size_t i = 0; i < lines.size(); i+=7) {
        auto m = std::make_shared<Monkey>();
        monkies_p1.push_back(m);

        // Get value of items
        split_line(lines[i+1], line);
        for (size_t j = 2; j < line.size(); j++)
            m->items.push_back(std::stoi(line[j]));

        // Get operation to perform
        split_line(lines[i+2], line);
        std::function<const uint64_t(const uint64_t, const uint64_t)> operation = line[4][0] == '+' ?
                (std::function<const uint64_t(const uint64_t, const uint64_t)>) std::plus<uint64_t>{} :
                (std::function<const uint64_t(const uint64_t, const uint64_t)>) std::multiplies<uint64_t>{};
        if (line[5][0] == 'o')
            m->op_func = [=](uint64_t item){ return operation(item, item); };
        else
            m->op_func = [=](uint64_t item){ return operation(item, std::stoi(line[5])); };

        // Get test to perform
        m->test_func = [=](uint64_t item){return item % std::stoi(lines[i+3].substr(21)) == 0 ?
                std::stoi(lines[i+4].substr(29)) : std::stoi(lines[i+5].substr(30)); };
    }

    auto monkies_p2 = monkies_p1;

    // P1
    for (size_t i = 0; i < 20; i++) {
        for (auto& monkey : monkies_p1)
            monkey->perform_turn(monkies_p1);
    }
    std::sort(monkies_p1.begin(), monkies_p1.end(), [](std::shared_ptr<Monkey> lhs, std::shared_ptr<Monkey> rhs)
        {return lhs->business > rhs->business;});
    std::cout << (monkies_p1[0]->business * monkies_p1[1]->business) << std::endl;

    return 0;
}
