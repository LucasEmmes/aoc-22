#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>


std::vector<std::string> aoc_read_lines_raw() {
    std::ifstream file("../input.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    return lines;
}

bool fits(std::vector<std::vector<char>>& chamber, int x, int y, int block) {
    if (y < 0) return false;
    bool result = true;
    if (block == 0 && x <= 3 && y >= 0) {
        for (size_t i = 0; i < 4; i++) result &= chamber[y][x+i] == '.';
        return result;
    } else if (block == 1 && x <= 4 && y >= 2) {
        for (size_t i = 0; i < 3; i++) result &= chamber[y-1][x+i] == '.';
        for (size_t i = 0; i < 3; i+=2) result &= chamber[y-i][x+1] == '.';
        return result;
    } else if (block == 2 && x <= 4 && y >= 2) {
        for (size_t i = 0; i < 3; i++) result &= chamber[y-2][x+i] == '.';
        for (size_t i = 0; i < 2; i++) result &= chamber[y-i][x+2] == '.';
        return result;
    } else if (block == 3 && y >= 3) {
        for (size_t i = 0; i < 4; i++) result &= chamber[y-i][x] == '.';
        return result;
    } else if (block == 4 && x <= 5 && y >= 1) {
        for (size_t i = 0; i < 4; i++) result &= chamber[y-(i/2)][x+(i%2)] == '.';
        return result;
    }
    return false;
}

uint64_t get_highest_rock(std::vector<std::vector<char>>& chamber) {
    for (int64_t y = chamber.size()-1; y >= 0; y--) {
        for (size_t x = 0; x < 7; x++) {
            if (chamber[y][x] == '#') return y;
        }
    }
    return -1;
}

void extend_chamber(std::vector<std::vector<char>>& chamber, int new_ceiling) {
    for (int i = chamber.size()-1; i < new_ceiling; i++) {
        chamber.push_back(std::vector<char>(7, '.'));
    }
}

void place_block_ref(std::vector<std::vector<char>>& chamber, int x, int y, int block) {
    if (block == 0) {
        for (size_t i = 0; i < 4; i++) chamber[y][x+i] = '#';
    } else if (block == 1) {
        for (size_t i = 0; i < 3; i++) chamber[y-1][x+i] = '#';
        for (size_t i = 0; i < 3; i+=2) chamber[y-i][x+1] = '#';
    } else if (block == 2) {
        for (size_t i = 0; i < 3; i++) chamber[y-2][x+i] = '#';
        for (size_t i = 0; i < 2; i++) chamber[y-i][x+2] = '#';
    } else if (block == 3) {
        for (size_t i = 0; i < 4; i++) chamber[y-i][x] = '#';
    } else if (block == 4) {
        for (size_t i = 0; i < 4; i++) chamber[y-(i/2)][x+(i%2)] = '#';
    }
}

std::vector<std::vector<char>> place_block(std::vector<std::vector<char>>& chamber, int x, int y, int block) {
    auto c = chamber;
    if (block == 0) {
        for (size_t i = 0; i < 4; i++) c[y][x+i] = '#';
    } else if (block == 1) {
        for (int i = 0; i < 3; i++) c[y-1][x+i] = '#';
        for (size_t i = 0; i < 3; i+=2) c[y-i][x+1] = '#';
    } else if (block == 2) {
        for (size_t i = 0; i < 3; i++) c[y-2][x+i] = '#';
        for (size_t i = 0; i < 2; i++) c[y-i][x+2] = '#';
    } else if (block == 3) {
        for (size_t i = 0; i < 4; i++) c[y-i][x] = '#';
    } else if (block == 4) {
        for (size_t i = 0; i < 4; i++) c[y-(i/2)][x+(i%2)] = '#';
    }
    return c;
}

void visualize(std::vector<std::vector<char>> chamber) {
    for (int64_t i = chamber.size()-1; i >= 0; i--) {
        for (auto& c : chamber[i]) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int check_pattern(std::vector<uint64_t>& heights) {
    std::vector<uint64_t> h1, h2;
    auto half = heights.size()/2;
    for (size_t i = 0; i < half; i++)
        h1.push_back(heights[i]);
    for (size_t i = 0; i < half; i++)
        h2.push_back(heights[i+half] - h1.back());
    if (std::equal(h1.begin(), h1.end(), h2.begin())) {
        return half;
    } else return 0;
}

int main() {

    // std::string line = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
    auto line = aoc_read_lines_raw()[0];
    uint64_t line_counter = 0;

    std::vector<std::vector<char>> chamber(5, std::vector<char>(7, '.'));
    std::vector<int> block_height = {1,3,3,4,2};

    // Contains the tower height after that block x has fallen
    std::vector<uint64_t> tower_heights = {};

    int current_block = 0;
    int block_x = 0;
    int block_y = 0;
    int movement = 0;
    for (size_t i = 0; i < 6000; i++) {
        block_x = 2;
        block_y = get_highest_rock(chamber) + 3 + block_height[current_block];
        extend_chamber(chamber, block_y);

        while (true) {
            movement = 0;
            // Check whether to move left or right
            if (line[line_counter%line.length()] == '>' && block_x < 6) movement = 1;
            else if (line[line_counter%line.length()] == '<' && block_x > 0) movement = -1;

            // Check if you are blocked by something
            if (fits(chamber, block_x+movement, block_y, current_block))
                block_x += movement;

            // Try to move down
            if (fits(chamber, block_x, block_y-1, current_block))
                block_y--;
            else {
                place_block_ref(chamber, block_x, block_y, current_block);
                line_counter++;
                tower_heights.push_back(get_highest_rock(chamber)+1);
                break;
            }
            line_counter++;
            // if (line_counter % line.length() == 0) {
            //     std::cout << "Found at counter " << line_counter << std::endl;
            //     std::cout << "Number of fallen blocks " << i << std::endl;
            //     std::cout << "Height of " << get_highest_rock(chamber)+1 << std::endl;
            //     std::cout << "Previous 4 heights:" << std::endl;
            //     for (size_t i = 0; i < 4; i++) std::cout << tower_heights[tower_heights.size()-i-2] << std::endl;
            //     std::vector<std::vector<char>> chamber_top;
            //     for (size_t i = chamber.size()-12; i < chamber.size(); i++) {
            //         chamber_top.push_back(chamber[i]);
            //     }
            //     visualize(chamber_top);
            // }
        }
        current_block = (current_block+1) % 5;

        // if (i == 2021) std::cout << get_highest_rock(chamber)+1 << std::endl;
        if (i == 1874) std::cout << get_highest_rock(chamber)+1 << std::endl;
    }

    // std::vector<uint64_t> test = {1,3,6,10,11,13,16,20};
    // std::cout << check_pattern(test) << std::endl;

    return 0;
}
