#include "../common/filereader.h"

#include <set>
#include <algorithm>

bool is_superset_of(std::set<int>& a, std::set<int>& b) {
	for (auto& element : a) {
		if (b.find(element) == b.end()) return false;
	}
	return true;
}


int main() {

	auto data = aoc_read_input_singles({'-', ','});
	std::vector<int> sections;
	for (auto& entry : data) sections.push_back(std::stoi(entry));

	int counter = 0;
	size_t length = sections.size();
	for (size_t i = 0; i < length; i+=4) {
		if (sections[i] <= sections[i+2] && sections[i+1] >= sections[i+3]) counter++;
		else if (sections[i+2] <= sections[i] && sections[i+3] >= sections[i+1]) counter++;
	}
	std::cout << counter << std::endl;


	counter = 0;
	for (size_t i = 0; i < length; i+=4) {
		if (sections[i] <= sections[i+2] && sections[i+1] >= sections[i+3]) counter++;
		else if (sections[i+2] <= sections[i] && sections[i+3] >= sections[i+1]) counter++;
		else if (sections[i] <= sections[i+2] && sections[i+1] >= sections[i+2]) counter++;
		else if (sections[i] <= sections[i+3] && sections[i+1] >= sections[i+3]) counter++;
	}
	std::cout << counter << std::endl;

	return 0;
}
