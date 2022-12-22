#include "../common/filereader.h"

#include <set>

std::set<std::string> can_reach;

std::string make_name(int x, int y, int z) {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

void floodfill(std::vector<std::vector<std::vector<bool>>>& cubes, int x, int y, int z) {
    auto name = make_name(x, y, z);

    // If this node has been checked in this exploration already, go back
    if (can_reach.count(name)) return;
    // Otherwise, search recursively

    // Take note that we are exploring from here
    can_reach.insert(name);

    for (size_t i = 0; i < 11; i+=2) {
        int dx = i<3 ? i-1 : 0;
        int dy = i>3 && i<7 ? i-5 : 0;
        int dz = i>7 && i<11 ? i-9 : 0;
        if (x+dx >= 0 && x+dx <= cubes.size()-1 &&
            y+dy >= 0 && y+dy <= cubes.size()-1 &&
            z+dz >= 0 && z+dz <= cubes.size()-1 &&
            cubes[x+dx][y+dy][z+dz] == false)
                // Check floodfill of every possible neighbouring node
                floodfill(cubes, x+dx,y+dy,z+dz);
    }
}

int main() {

    auto lines = aoc_read_input_lines_split({','});

    auto cubes = std::vector<std::vector<std::vector<bool>>>(25,
        std::vector<std::vector<bool>>(25,
            std::vector<bool>(25)));

    int max_x = 0;
    int max_y = 0;
    int max_z = 0;
    for (auto& line : lines) {
        int x = std::stoi(line[0]);
        int y = std::stoi(line[1]);
        int z = std::stoi(line[2]);
        cubes[x+1][y+1][z+1] = true;
        if (max_x < x) max_x = x;
        if (max_y < y) max_y = y;
        if (max_z < z) max_z = z;
    }

    // P1
    uint32_t faces = 0;
    for (size_t x = 0; x < max_x + 2; x++) {
        for (size_t y = 0; y < max_y + 2; y++) {
            for (size_t z = 0; z < max_z + 2; z++) {
                if (cubes[x][y][z] == false) continue;
                if (x > 0 && cubes[x-1][y][z] == false) faces++;
                if (x < cubes.size() && cubes[x+1][y][z] == false) faces++;
                if (y > 0 && cubes[x][y-1][z] == false) faces++;
                if (y < cubes.size() && cubes[x][y+1][z] == false) faces++;
                if (z > 0 && cubes[x][y][z-1] == false) faces++;
                if (z < cubes.size() && cubes[x][y][z+1] == false) faces++;
            }
        }
    }
    std::cout << faces << std::endl;

    // P2
    floodfill(cubes,0,0,0);
    faces = 0;
    for (size_t x = 0; x < max_x + 2; x++) {
        for (size_t y = 0; y < max_y + 2; y++) {
            for (size_t z = 0; z < max_z + 2; z++) {
                if (cubes[x][y][z] == false) continue;
                // Go over each of the 6 sides
                for (size_t i = 0; i < 11; i+=2) {
                auto dx = i<3 ? i-1 : 0;
                auto dy = i>3 && i<7 ? i-5 : 0;
                auto dz = i>7 && i<11 ? i-9 : 0;
                if (x > 0 && x < cubes.size()-1 &&
                    y > 0 && y < cubes.size()-1 &&
                    z > 0 && z < cubes.size()-1 &&
                    cubes[x+dx][y+dy][z+dz] == false)
                        faces += can_reach.count(make_name(x+dx,y+dy,z+dz));
                }
            }
        }
    }
    std::cout << faces << std::endl;

    return 0;
}