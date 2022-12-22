#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <unordered_map>

std::vector<std::vector<std::string>> aoc_read_input_lines_split(std::vector<char> splits) {
    std::vector<std::string> lines;
    std::vector<std::vector<std::string>> data;
    std::ifstream input_file("../input.txt");
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

enum model {ore, clay, obsidian, geode};
std::vector<model> models = {model::ore, model::clay, model::obsidian, model::geode};

struct Resources {
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;
};
bool operator<=(const Resources lhs, const Resources rhs) {
    return lhs.ore <= rhs.ore &&
        lhs.clay <= rhs.clay &&
        lhs.obsidian <= rhs.obsidian &&
        lhs.geode <= rhs.geode;
}
void operator-=(Resources& lhs, Resources rhs) {
    lhs.ore -= rhs.ore;
    lhs.clay -= rhs.clay;
    lhs.obsidian -= rhs.obsidian;
    lhs.geode -= rhs.geode;
}
void operator+=(Resources& lhs, Resources rhs) {
    lhs.ore += rhs.ore;
    lhs.clay += rhs.clay;
    lhs.obsidian += rhs.obsidian;
    lhs.geode += rhs.geode;
}

class Recipe {
public:
    std::unordered_map<model, Resources> costs;
    Recipe(std::vector<Resources> costs) {
        for (size_t i = 0; i < 4; i++)
            this->costs[models[i]] = costs[i];
    };
    bool purchase(model m, Resources& wallet) {
        auto price = costs[m];
        if (price <= wallet) {
            wallet -= price;
            return true;
        } else {
            return false;
        }
    }
};

int64_t rec(Recipe& recipe, Resources wallet, Resources robots, Resources factory, int minutes_left) {
    if (minutes_left == 0) return wallet.geode;
    
    // Gain materials
    wallet += robots;

    // Finish producing robots
    robots += factory;
    factory = Resources();

    // For every combination, try
    std::vector<int64_t> geodes_collected;
    geodes_collected.push_back(rec(recipe, wallet, robots, factory, minutes_left-1));
    for (auto& m : models) {
        auto wallet_copy = wallet;
        if (recipe.purchase(m, wallet_copy)) {
            auto factory_copy = factory;
            if (m == model::ore) factory_copy.ore++;
            else if (m == model::clay) factory_copy.clay++;
            else if (m == model::obsidian) factory_copy.obsidian++;
            else if (m == model::geode) factory_copy.geode++;
            geodes_collected.push_back(rec(recipe, wallet_copy, robots, factory_copy, minutes_left-1));
        }
    }

    std::sort(geodes_collected.begin(), geodes_collected.end());
    return geodes_collected.back();
}

int main(int argc, char** argv) {

    auto lines = aoc_read_input_lines_split({':'});

    // Setup
    std::vector<Recipe> recipes;
    for (auto& line : lines) {
        Resources orebot;
        orebot.ore = std::stoi(line[6]);

        Resources claybot;
        claybot.ore = std::stoi(line[12]);
        
        Resources obsidianbot;
        obsidianbot.ore = std::stoi(line[18]);
        obsidianbot.clay = std::stoi(line[21]);
        
        Resources geodebot;
        geodebot.clay = std::stoi(line[27]);
        geodebot.obsidian = std::stoi(line[30]);

        Recipe recipe({orebot, claybot, obsidianbot, geodebot});
        recipes.push_back(recipe);
    }

    Resources wallet;
    Resources factory;
    Resources robots;
    robots.ore++;
    std::cout << rec(recipes.back(), wallet, robots, factory, argc > 1 ? std::stoi(argv[1]) : 1) << std::endl;

    return 0;
}
