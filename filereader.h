#ifndef AOC_FILEREADER
#define AOC_FILEREADER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

void aoc_read_input(std::vector<std::string>& data, std::vector<char>& splits, bool end=true) {
    std::vector<std::string> lines;
    std::ifstream input_file("input.txt");
    std::string temp;
    if (input_file.is_open()) {
        while (input_file) {
            std::getline(input_file, temp);
            for (auto& s : splits) std::replace(temp.begin(), temp.end(), s, ' ');
            std::stringstream ss(temp);
            while (ss >> temp) data.push_back(temp);
        }
    }
}

#endif