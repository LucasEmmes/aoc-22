#include "../common/filereader.h"

#include <unordered_map>

std::unordered_map<std::string, bool> can_reach;
std::unordered_map<std::string, bool> current_path;

std::string make_name(int x, int y, int z) {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

bool recursive_search(std::vector<std::vector<std::vector<bool>>>& cubes, int x, int y, int z) {
    // If we are at 0,0,0 then we have reached our goal
    // and any node along the path can reach it as well
    if (x+y+z == 0) return true;
    auto name = make_name(x, y, z);

    // If this node has been checked in this exploration already, go back
    if (current_path.find(name) != current_path.end()) return false;
    
    // If we know whether or not this node reaches air, we return it
    if (can_reach.find(name) != can_reach.end()) return can_reach[name];
    // Otherwise, search recursively


    // Take note that we are exploring from here
    current_path[name] = true;
    
    for (size_t i = 0; i < 11; i+=2) {
        auto dx = i<3 ? i-1 : 0;
        auto dy = i>3 && i<7 ? i-5 : 0;
        auto dz = i>7 && i<11 ? i-9 : 0;

        if (x > 0 && x < cubes.size()-1 &&
            y > 0 && y < cubes.size()-1 &&
            z > 0 && z < cubes.size()-1 &&
            cubes[x+dx][y+dy][z+dz] == false) {
                auto dest_name = make_name(x+dx,y+dy,z+dz);
                auto dest_can_reach = recursive_search(cubes, x+dx,y+dy,z+dz);
                // Assuming the node next to us can reach air, it means we can as well
                if (dest_can_reach) {
                    can_reach[name] = true;
                    // Remember to clean up path as we go up
                    current_path.erase(name);
                    return true;
                }
                // If not, we continue searching the other remaining directions
            }
    }
    // Remember to clean up path as we go up
    current_path.erase(name);
    // If none of the cubes next to us managed to reach air, then neither can we
    can_reach[name] = false;
    return false;
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
    faces = 0;
    for (size_t x = 0; x < max_x + 2; x++) {
        for (size_t y = 0; y < max_y + 2; y++) {
            for (size_t z = 0; z < max_z + 2; z++) {
                if (cubes[x][y][z] == false) continue;
                if (x > 0 && cubes[x-1][y][z] == false)             faces += recursive_search(cubes, x-1, y, z);
                if (x < cubes.size() && cubes[x+1][y][z] == false)  faces += recursive_search(cubes, x+1, y, z);
                if (y > 0 && cubes[x][y-1][z] == false)             faces += recursive_search(cubes, x, y+1, z);
                if (y < cubes.size() && cubes[x][y+1][z] == false)  faces += recursive_search(cubes, x, y-1, z);
                if (z > 0 && cubes[x][y][z-1] == false)             faces += recursive_search(cubes, x, y, z+1);
                if (z < cubes.size() && cubes[x][y][z+1] == false)  faces += recursive_search(cubes, x, y, z-1);
            }
        }
    }
    std::cout << faces << std::endl;

    return 0;
}