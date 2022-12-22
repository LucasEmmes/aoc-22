#include "../common/filereader.h"

#include <iostream>
#include <functional>
#include <queue>
#include <memory>

void split_line(std::string& line, std::vector<std::string>& collector) {
    collector.clear();
    std::stringstream ss(line);
    std::string temp_str;
    while (ss >> temp_str) collector.push_back(temp_str);
}

class Monkey{
public:
	int business = 0;
	std::queue<int> items;
	std::function<int(int)> op_func;
	std::function<int(int)> test_func;
	void perform_turn(std::vector<std::shared_ptr<Monkey>>& monkeys) {
		while (items.size()) {
			int item = items.front();
			// Perform operation
			item = op_func(item);
			// Calm down
			item = item/3;
			// Throw to next monkey
			int next_monkey = test_func(item);
			monkeys[next_monkey]->items.push(item);
			items.pop();
			business++;
		}
	}
};

int main() {

	auto lines = aoc_read_lines_raw();
	std::vector<std::shared_ptr<Monkey>> monkeys;

	std::vector<std::string> line;
	for (size_t i = 0; i < lines.size(); i+=7) {
		monkeys.push_back(std::make_shared<Monkey>());
		int m = i/7;

		split_line(lines[i+1], line);
		for (size_t j = 2; j < line.size(); j++)
				monkeys[m]->items.push(std::stoi(line[j]));
		split_line(lines[i+2], line);
		if (line[4] == "+")
			monkeys[m]->op_func = [=](int old){return old + std::stoi(line[6]);};
		else if (line[4] == "*")
			monkeys[m]->op_func = [=](int old){return old * std::stoi(line[6]);};
		split_line(lines[i+3], line);
		auto if_true = lines[i+4];
		auto if_false = lines[i+5];
		monkeys[m]->test_func = [=](int item){
			if (item % std::stoi(line[3]) == 0) {
				int monkey = std::stoi(if_true.substr(29)); 
				return monkey; 
			}
			else {
				int monkey = std::stoi(if_false.substr(30));
				return monkey;
			}
		};
		
	}

	
	for (size_t i = 0; i < 20; i++) {
		for (auto& monkey : monkeys) {
			monkey->perform_turn(monkeys);
		}
	}

	for (auto& monkey : monkeys) printf("%d\n", monkey->business);

	std::sort(monkeys.begin(), monkeys.end(),
			[](std::shared_ptr<Monkey> lhs, std::shared_ptr<Monkey> rhs){return lhs->business > rhs->business;});
	int monkey_business = monkeys[0]->business * monkeys[1]->business;
	printf("%d\n", monkey_business);

}

