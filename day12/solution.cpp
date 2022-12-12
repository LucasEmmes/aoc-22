#include "../common/filereader.h"
#include "../common/pathfinding/astar.hpp"

std::string make_string(int y, int x) {
    return std::to_string(y) + "," + std::to_string(x);
}

int main() {
    auto lines = aoc_read_lines_raw();

    Pathfinder::AStar<std::string> astar;
    using Edge = Pathfinder::Graph_Edge<std::string>;
    std::vector<Edge> edges;

    std::string start;
    std::string end;
    std::vector<std::string> scenic_starts;


    for (size_t y = 0; y < lines.size(); y++) {
        for (size_t x = 0; x < lines[0].size(); x++) {
            if (lines[y][x] == 'S') {
                start = make_string(y, x);
                lines[y].replace(x, 1, "a");
            }
            if (lines[y][x] == 'E') {
                end = make_string(y, x);
                lines[y].replace(x, 1, "z");
            }
            if (lines[y][x] == 'a') scenic_starts.push_back(make_string(y, x));
            if (y > 0 && lines[y][x]+1 >= lines[y-1][x])
                edges.push_back(Edge(make_string(y, x), make_string(y-1, x), 1));
            if (y < lines.size()-1 && lines[y][x]+1 >= lines[y+1][x])
                edges.push_back(Edge(make_string(y, x), make_string(y+1, x), 1));
            if (x > 0 && lines[y][x]+1 >= lines[y][x-1])
                edges.push_back(Edge(make_string(y, x), make_string(y, x-1), 1));
            if (x < lines[0].size()-1 && lines[y][x]+1 >= lines[y][x+1])
                edges.push_back(Edge(make_string(y, x), make_string(y, x+1), 1));
        }
    }

    astar.make_graph_from_edges(edges);

    // P1
    astar.search_for_path(start, end);
    std::cout << astar.get_cost() << std::endl;

    // P2
    std::vector<uint64_t> path_lengths;
    for (auto& scenic_start : scenic_starts) {
        if (astar.search_for_path(scenic_start, end))
            path_lengths.push_back(astar.get_cost());
    }
    std::sort(path_lengths.begin(), path_lengths.end());
    std::cout << path_lengths.front() << std::endl;
}