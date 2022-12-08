#include "../common/filereader.h"

int main() {
    auto lines = aoc_read_lines_raw();

    std::vector<std::vector<int8_t>> forest(lines.size(), std::vector<int8_t>(lines[0].length()));
    for (size_t i = 0; i < lines.size(); i++)
        for (size_t j = 0; j < lines[0].size(); j++)
            forest[i][j] = lines[i][j]-48;
        
    int8_t max_val = -1;
    
    auto forest_lr = forest;
    for (size_t y = 1; y < forest.size()-1; y++) {
        max_val = forest[y][0];
        for (size_t x = 1; x < forest[0].size()-1; x++) {
            if (forest_lr[y][x] > max_val) {
                max_val = forest_lr[y][x];
                forest_lr[y][x] = -1;
            }
        }
    }

    auto forest_rl = forest;
    for (size_t y = forest.size()-1; y >= 1; y--) {
        max_val = forest[y][forest[0].size()-1];
        for (size_t x = forest[0].size()-1; x >= 1; x--) {
            if (forest_rl[y][x] > max_val) {
                max_val = forest_rl[y][x];
                forest_rl[y][x] = -1;
            }
        }
    }

    auto forest_bt = forest;
    for (size_t x = forest[0].size()-1; x >= 1; x--) {
        max_val = forest[forest.size()-1][x];
        for (size_t y = forest.size()-1; y >= 1; y--) {
            if (forest_bt[y][x] > max_val) {
                max_val = forest_bt[y][x];
                forest_bt[y][x] = -1;
            }
        }
    }
    
    auto forest_tb = forest;
    for (size_t x = 1; x < forest[0].size()-1; x++) {
        max_val = forest[0][x];
        for (size_t y = 1; y < forest.size()-1; y++) {
            if (forest_tb[y][x] > max_val) {
                max_val = forest_tb[y][x];
                forest_tb[y][x] = -1;
            }
        }
    }

    uint64_t visible = forest.size() * forest[0].size();
    for (size_t x = 1; x < forest[0].size()-1; x++) {
        for (size_t y = 1; y < forest.size()-1; y++) {
            if (forest_bt[y][x]>-1 && forest_tb[y][x]>-1 && forest_rl[y][x]>-1 && forest_lr[y][x]>-1) {
                visible--;
            }
        }
    }

    uint64_t highscore = 0;
    uint64_t score;
    uint8_t height, count;
    for (size_t y = 1; y < forest.size()-1; y++) {
        for (size_t x = 1; x < forest[0].size()-1; x++) {
            score = 1;
            height = forest[y][x];
            for (size_t lx = x+1; lx < forest[0].size(); lx++) {
                count++;
                if (!forest[y][lx]) continue;
                if (forest[y][lx] >= height) break;
            }
            score*=count;
            count = 0;
            for (int rx = x-1; rx >= 0; rx--) {
                count++;
                if (!forest[y][rx]) continue;
                if (forest[y][rx] >= height) break;
            }
            score*=count;
            count = 0;
            for (size_t by = y+1; by < forest.size(); by++) {
                count++;
                if (!forest[by][x]) continue;
                if (forest[by][x] >= height) break;
            }
            score*=count;
            count = 0;
            for (int ty = y-1; ty >= 0; ty--) {
                count++;
                if (!forest[ty][x]) continue;
                if (forest[ty][x] >= height) break;
            }
            score*=count;
            count = 0;
            if (score>highscore) highscore = score;
        }
    }
    
    
    std::cout << visible << std::endl;
    std::cout << highscore << std::endl;
    return 0;
}