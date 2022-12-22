#include "../common/filereader.h"
#include "../common/pathfinding/astar.hpp"

std::string make_name(int x, int y, int z) {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

bool recursive_search(std::vector<std::vector<std::vector<bool>>>& cubes, int x, int y, int z) {
    if (x+y+z == 0) return true;
    cubes[x][y][z] = true;
    bool result;
    if (x > 0 && cubes[x-1][y][z] == false)             if ((result = recursive_search(cubes, x-1,y,z))) {
        cubes[x][y][z] = false;
        return result;}
    if (x < cubes.size()-1 && cubes[x+1][y][z] == false)  if ((result = recursive_search(cubes, x+1,y,z))) {
        cubes[x][y][z] = false;
        return result;}
    if (y > 0 && cubes[x][y-1][z] == false)             if ((result = recursive_search(cubes, x,y-1,z))) {
        cubes[x][y][z] = false;
        return result;}
    if (y < cubes.size()-1 && cubes[x][y+1][z] == false)  if ((result = recursive_search(cubes, x,y+1,z))) {
        cubes[x][y][z] = false;
        return result;}
    if (z > 0 && cubes[x][y][z-1] == false)             if ((result = recursive_search(cubes, x,y,z-1))) {
        cubes[x][y][z] = false;
        return result;}
    if (z < cubes.size()-1 && cubes[x][y][z+1] == false)  if ((result = recursive_search(cubes, x,y,z+1))) {
        cubes[x][y][z] = false;
        return result;}
    cubes[x][y][z] = false;
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

    std::vector<Pathfinder::Graph_Edge<std::string>> edges;

    // P1 + prep for P2
    uint32_t faces = 0;
    for (size_t x = 0; x < max_x + 2; x++) {
        for (size_t y = 0; y < max_y + 2; y++) {
            for (size_t z = 0; z < max_z + 2; z++) {
                if (cubes[x][y][z] == false) {
                    // Look in all directions to make edges for graph
                    if (x > 0 && cubes[x-1][y][z] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x-1,y,z), 1));
                    if (x < cubes.size() && cubes[x+1][y][z] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x+1,y,z), 1));
                    if (y > 0 && cubes[x][y-1][z] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x,y-1,z), 1));
                    if (y < cubes.size() && cubes[x][y+1][z] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x,y+1,z), 1));
                    if (z > 0 && cubes[x][y][z-1] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x,y,z-1), 1));
                    if (z < cubes.size() && cubes[x][y][z+1] == false) edges.push_back(Pathfinder::Graph_Edge<std::string>(make_name(x,y,z), make_name(x,y,z+1), 1));
                    continue;
                }
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
    auto alg = Pathfinder::AStar<std::string>();
    alg.make_graph_from_edges(edges);

    faces = 0;
    int counter = 0;
    for (size_t x = 0; x < max_x + 2; x++) {
        for (size_t y = 0; y < max_y + 2; y++) {
            for (size_t z = 0; z < max_z + 2; z++) {
                if (counter++%100==0) std::cout << counter << std::endl;
                if (cubes[x][y][z] == false) continue;
                if (x > 0 && cubes[x-1][y][z] == false)             faces += recursive_search(cubes, x-1, y, z);
                if (x < cubes.size() && cubes[x+1][y][z] == false)  faces += recursive_search(cubes, x+1, y, z);
                if (y > 0 && cubes[x][y-1][z] == false)             faces += recursive_search(cubes, x, y+1, z);
                if (y < cubes.size() && cubes[x][y+1][z] == false)  faces += recursive_search(cubes, x, y-1, z);
                if (z > 0 && cubes[x][y][z-1] == false)             faces += recursive_search(cubes, x, y, z+1);
                if (z < cubes.size() && cubes[x][y][z+1] == false)  faces += recursive_search(cubes, x, y, z-1);
                // if (x > 0 && cubes[x-1][y][z] == false) faces += alg.search_for_path("0,0,0", make_name(x-1, y, z));
                // if (x < cubes.size() && cubes[x+1][y][z] == false)  faces += alg.search_for_path("0,0,0", make_name(x+1, y, z));
                // if (y > 0 && cubes[x][y-1][z] == false)  faces += alg.search_for_path("0,0,0", make_name(x, y+1, z));
                // if (y < cubes.size() && cubes[x][y+1][z] == false)  faces += alg.search_for_path("0,0,0", make_name(x, y-1, z));
                // if (z > 0 && cubes[x][y][z-1] == false)  faces += alg.search_for_path("0,0,0", make_name(x, y, z+1));
                // if (z < cubes.size() && cubes[x][y][z+1] == false)  faces += alg.search_for_path("0,0,0", make_name(x, y, z-1));
            }
        }
    }
    std::cout << faces << std::endl;

    return 0;
}