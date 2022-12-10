#include "../common/filereader.h"

void check(std::vector<std::vector<char>>& crt, int& cycle, int& cpu, int& score) {
	if (cycle % 40 == 20) score += cycle * cpu;
	if (cycle <= 240 && (cycle-1)%40 > cpu-2 && (cycle-1)%40 < cpu+2) crt[(cycle-1)/40][(cycle-1)%40] = '#';
	cycle++;
}

int main() {
	auto lines = aoc_read_lines_raw();

	int cpu = 1;
	int cycle = 1;
	int score = 0;

	std::vector<std::vector<char>> crt(6, std::vector<char>(40, '.'));

	for (size_t i = 0; i < lines.size(); i++) {
		check(crt, cycle, cpu, score);
		if (lines[i][0] == 'a') {
			check(crt, cycle, cpu, score);
			cpu += std::stoi(lines[i].substr(5));
		}
	}
	printf("%d\n", score);
	for (auto& row : crt) {
		for (auto& c : row)
			printf("%c", c);
		printf("\n");
	}
}
