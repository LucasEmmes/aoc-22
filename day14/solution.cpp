#include "../common/filereader.h"

bool drop_sand(std::vector<std::vector<int>>& grid) {
    int sx = 500;
    int sy = 0;

    while (sy < grid.size()-1) {
        if (grid[sy+1][sx] == 0)
            sy++;
        else if (sx > 0 && grid[sy+1][sx-1] == 0) {
            sy++;
            sx--;
        } else if (sx < grid[0].size()-1 && grid[sy+1][sx+1] == 0) {
            sy++;
            sx++;
        } else {
            grid[sy][sx] = 2;
            return true;
        }
    }
    return false;
}

int main() {
    auto lines_2 = aoc_read_input_lines_split({',', '-', '>'});
    
    std::vector<std::vector<int>> grid;
    for (size_t i = 0; i < 200; i++) {
        std::vector<int> row;
        for (size_t j = 0; j < 1200; j++)
            row.push_back(0);
        grid.push_back(row);
    }

    int max_y = 0;
    for (auto& coords : lines_2) {
        for (size_t i = 0; i < coords.size()-2; i+=2) {
            auto x1 = std::stoi(coords[i]);
            auto y1 = std::stoi(coords[i+1]);
            auto x2 = std::stoi(coords[i+2]);
            auto y2 = std::stoi(coords[i+3]);
            if (x1 == x2) {
                auto min = y1*(y1<y2) + y2*(y2<y1);
                auto max = y1*(y1>y2) + y2*(y2>y1);
                for (size_t y = min; y <= max; y++)
                    grid[y][x1] = 1;
                if (max > max_y) max_y = max;
            } else if (y1 == y2) {
                auto min = x1*(x1<x2) + x2*(x2<x1);
                auto max = x1*(x1>x2) + x2*(x2>x1);
                for (size_t x = min; x <= max; x++)
                    grid[y1][x] = 1;
            }
        }
    }

    // P1
    int sand = 0;
    while (true) {
        if (!drop_sand(grid)) break;
        sand++;
    }
    std::cout << sand << std::endl;

    // P2
    max_y+=2;
    for (size_t i = 0; i < 1200; i++) grid[max_y][i] = 1;

    while (grid[0][500] == 0) {
        drop_sand(grid);
        sand++;
    }
    std::cout << sand << std::endl;
}