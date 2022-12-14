#ifndef AOC_FILEREADER
#define AOC_FILEREADER

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

std::vector<std::string> aoc_read_input_singles(std::vector<char> splits) {
    std::vector<std::string> lines;
    std::vector<std::string> data;
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
    return data;
}

std::vector<std::vector<std::string>> aoc_read_input_lines_split(std::vector<char> splits) {
    std::vector<std::string> lines;
    std::vector<std::vector<std::string>> data;
    std::ifstream input_file("input.txt");
    std::string temp;
    if (input_file.is_open()) {
        while (input_file) {
            std::vector<std::string> tempvec;
            std::getline(input_file, temp);
            for (auto& s : splits) std::replace(temp.begin(), temp.end(), s, ' ');
            std::stringstream ss(temp);
            while (ss >> temp) tempvec.push_back(temp);
            if (tempvec.size()) data.push_back(tempvec);
        }
    }
    return data;
}

std::vector<std::string> aoc_read_lines_raw() {
    std::ifstream file("input.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    return lines;
}

#endif
