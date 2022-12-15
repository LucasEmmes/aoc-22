#include "../common/filereader.h"
#include <cmath>
#include <set>
struct Pos {
    int x;
    int y;
};

struct Sensor {
    int x;
    int y;
    int r;
};

int main() {
    auto lines = aoc_read_input_lines_split({'='});
    int yline = 2000000;

    std::vector<Sensor> sensors;
    uint64_t impossible_positions = 0;
    std::vector<std::pair<int64_t, int64_t>> ranges;

    for (auto& line : lines) {
        Pos sensor = {std::stoi(line[3]), std::stoi(line[5])};
        Pos beacon = {std::stoi(line[11]), std::stoi(line[13])};
        int distance = std::abs(sensor.x - beacon.x) + std::abs(sensor.y - beacon.y);
        sensors.push_back({sensor.x, sensor.y, distance});
        if (distance >= std::abs(sensor.y - yline)) {
            int64_t dtb = (distance - std::abs(sensor.y - yline));
            int64_t start = (sensor.x - dtb);
            int64_t stop = (sensor.x + dtb);
            ranges.push_back(std::make_pair(start, stop));
        }
    }

    std::sort(ranges.begin(), ranges.end());
    
    int start = ranges[0].first;
    int stop = ranges[0].second;
    for (size_t i = 1; i < ranges.size(); i++) {
        auto p = ranges[i];
        if (p.first > stop) {
            impossible_positions += stop - start;
            start = p.first;
            stop = p.second;
        } else {
            if (p.second > stop) stop = p.second;
        }
    }
    impossible_positions += stop - start;
    std::cout << impossible_positions << std::endl;
    
    
    


    return 0;
}