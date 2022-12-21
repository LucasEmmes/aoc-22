#include "../common/filereader.h"

struct Resources {
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geodes = 0;
};

struct Recipe {
    Resources orebot;
    Resources claybot;
    Resources obsidianbot;
    Resources geodebot;
};

enum Rob {Ore, Clay, Obsidian, Geode};

std::vector<Rob> models = {Rob::Ore, Rob::Clay, Rob::Obsidian, Rob::Geode};

void purchase(Recipe& recipe, Resources& inventory, Rob robot) {
    if (robot == Rob::Ore) {
        inventory.ore -= recipe.orebot.ore;
    } else if (robot == Rob::Clay) {
        inventory.ore -= recipe.claybot.ore;
    } else if (robot == Rob::Obsidian) {
        inventory.ore -= recipe.obsidianbot.ore;
        inventory.clay -= recipe.obsidianbot.clay;
    } else if (robot == Rob::Geode) {
        inventory.clay -= recipe.geodebot.clay;
        inventory.obsidian -= recipe.geodebot.obsidian;
    }
}

bool can_afford(Recipe& recipe, Resources& inventory, Rob robot) {
    if (robot == Rob::Ore) {
        return (recipe.orebot.ore <= inventory.ore);
    } else if (robot == Rob::Clay) {
        return (recipe.claybot.ore <= inventory.ore);
    } else if (robot == Rob::Obsidian) {
        return (recipe.obsidianbot.ore <= inventory.ore) && (recipe.claybot.clay <= inventory.clay);
    } else if (robot == Rob::Geode) {
        return (recipe.geodebot.clay <= inventory.clay) && (recipe.geodebot.obsidian <= inventory.obsidian);
    }
    return false;
}

int maximize(Recipe& recipe, Resources inventory, Resources robots, std::vector<Rob> factory, int minutes_left) {
    if (minutes_left == 0) return inventory.geodes;
    
    // Gather ores
    inventory.ore += robots.ore;
    inventory.clay += robots.clay;
    inventory.obsidian += robots.obsidian;
    inventory.geodes += robots.geodes;

    // Finish production
    for (auto& r : factory) {
        if (r == Rob::Ore) robots.ore++;
        if (r == Rob::Clay) robots.clay++;
        if (r == Rob::Obsidian) robots.obsidian++;
        if (r == Rob::Geode) robots.geodes++;
    }
    factory.clear();

    // See what we can buy
    std::vector<int> possible_production = {0};
    possible_production.push_back(maximize(recipe, inventory, robots, factory, minutes_left-1));
    for (auto& m : models) {
        auto inv = inventory;
        auto robs = robots;
        auto fact = factory;
        if (can_afford(recipe, inv, m)) {
            purchase(recipe, inv, m);
            fact.push_back(m);
            possible_production.push_back(maximize(recipe, inv, robs, fact, minutes_left-1));
        }
    }
    
    std::sort(possible_production.begin(), possible_production.end());
    return possible_production.back();
}


int main() {

    auto lines = aoc_read_input_lines_split({':'});

    // Setup
    std::vector<Recipe> recipes;
    for (auto& line : lines) {
        Recipe r;
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
        
        r.orebot = orebot;
        r.claybot = claybot;
        r.obsidianbot = obsidianbot;
        r.geodebot = geodebot;

        recipes.push_back(r);
    }

    Resources inventory;
    Resources robots;
    robots.ore = 1;
    std::vector<Rob> factory;


    // P1
    // std::cout << can_afford(recipes.back(), inventory, Rob::Ore) << std::endl;
    // inventory.ore = 10;
    // std::cout << can_afford(recipes.back(), inventory, Rob::Ore) << std::endl;

    // while (can_afford(recipes.back(), inventory, Rob::Ore))
    //     purchase(recipes.back(), inventory, Rob::Ore);

    auto test = maximize(recipes.front(), inventory, robots, {}, 10);
    std::cout << test << std::endl;

    return 0;
}