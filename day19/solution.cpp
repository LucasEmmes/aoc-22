#include "../common/filereader.h"

#include <unordered_map>

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

int main() {

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
    Resources robots;

    return 0;
}