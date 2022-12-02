#include <set>
#include <map>

#include "../common/filereader.h"

int main() {

    auto matches = aoc_read_lines_raw();

    std::map<char, int> values = {{'X', 1}, {'Y', 2}, {'Z', 3}};

    // P1
    std::set<std::string> wins = {"A Y", "B Z", "C X"};
    std::set<std::string> ties = {"A X", "B Y", "C Z"};
    std::set<std::string> loses = {"A Z", "B X", "C Y"};
    
    size_t score = 0;
    for (auto& match : matches) {
        if (wins.find(match) != wins.end()) score += 6;
        if (ties.find(match) != ties.end()) score += 3;
        score += values[match.back()];
    }
    std::cout << score << std::endl;

    // P2
    std::map<char, char> to_win = {{'A', 'Y'}, {'B', 'Z'}, {'C', 'X'}};
    std::map<char, char> to_tie = {{'A', 'X'}, {'B', 'Y'}, {'C', 'Z'}};
    std::map<char, char> to_lose = {{'A', 'Z'}, {'B', 'X'}, {'C', 'Y'}};

    score = 0;
    for (auto& match : matches) {
        if (match.back() == 'Z') score += 6 + values[to_win[match.front()]];
        if (match.back() == 'Y') score += 3 + values[to_tie[match.front()]];
        if (match.back() == 'X') score += values[to_lose[match.front()]];
    }
    std::cout << score << std::endl;

    return 0;
}