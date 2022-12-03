#include "../common/filereader.h"

#include <unordered_map>

uint64_t char_to_priority(char c) {
	return (c < 91)*(c-38) + (c>96)*(c-96);
}

// Turn a character into a bitstring where
// the bit at posiiton "priority" is 1
// e.g. 'c' -> ...0100
uint64_t char_to_bits(char c) {
	uint64_t bits = 1;
	return bits << (char_to_priority(c) - 1);
}

int get_bit_pos(size_t n) {
	int pos = 0;
	while (n%2 == 0) {
		n /= 2;
		pos++;
	}
	return pos;
}

// For every character, set the bit on its half
// to 1, then we can check for overlap with bitwise &
uint64_t find_common_in_compartments(std::string& line) {
	uint64_t left = 0;
	uint64_t right = 0;
	uint64_t bit_overlap;
	
	uint64_t halfway = line.length() / 2;
	for (uint64_t i = 0; i < halfway; i++) {
		left |= char_to_bits(line[i]);
		right |= char_to_bits(line[i+halfway]);
		bit_overlap = left & right;
		if (bit_overlap)
			return get_bit_pos(bit_overlap) + 1;
	}
	return 0;
}

// Get the bitmask of every backpack, then bitwise AND them all
uint64_t find_common_in_backpacks(std::vector<std::string> group) {
	uint64_t bit_indexes[] = {0,0,0};
	for (size_t i = 0; i < 3; i++) {
		for (auto& c : group[i]) 
			bit_indexes[i] |= char_to_bits(c);
	}
	uint64_t bits_overlap = bit_indexes[0] & bit_indexes[1] & bit_indexes[2];
	return get_bit_pos(bits_overlap) + 1;
}

int main() {
	auto lines = aoc_read_lines_raw();
	lines.pop_back();

	// P1
	uint64_t sum = 0;
	for (auto& backpack : lines)
		sum += find_common_in_compartments(backpack);
	std::cout << sum << std::endl;


	// P2
	int length = lines.size();
	sum = 0;
	for (size_t i = 0; i < length; i += 3)
		sum += find_common_in_backpacks({lines[i], lines[i+1], lines[i+2]});
	std::cout << sum << std::endl;

	return 0;	
}

