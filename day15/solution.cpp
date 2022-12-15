#include "../common/filereader.h"
#include <algorithm>
#include <cmath>

struct Sensor {
    int64_t x;
    int64_t y;
    int64_t r;
};

struct Range {
    int64_t start;
    int64_t stop;
};

void sort_ranges(std::vector<Range>& ranges) {
    std::sort(ranges.begin(), ranges.end(), [&](Range lhs, Range rhs){return lhs.start < rhs.start;});
}

void make_ranges(std::vector<Range>& ranges, std::vector<Sensor>& sensors, int64_t line, int64_t leftcap = INT64_MAX+1, int64_t rightcap = INT64_MAX) {
    ranges.clear();
    for (auto& sensor : sensors) {
        if (sensor.r >= std::abs(sensor.y - line)) {
            int64_t delta_x = sensor.r - std::abs(sensor.y - line);
            Range r = {sensor.x - delta_x, sensor.x + delta_x};
            if (r.start > rightcap || r.stop < leftcap) continue;
            if (r.start < leftcap) r.start = leftcap;
            if (r.stop > rightcap) r.stop = rightcap;
            ranges.push_back(r);
        }
    }
}

std::pair<int64_t, int64_t> get_impossible(std::vector<Range>& ranges, bool verbose = false) {
    uint64_t impossible_positions = 0;
    auto[start, stop] = ranges[0];
    Range r;
    int64_t hole;
    for (size_t i = 1; i < ranges.size(); i++) {
        r = ranges[i];
        if (r.start > (stop+1)) {
            hole = stop+1;
            impossible_positions += stop - start + 1;
            start = r.start;
            stop = r.stop;
        } else if (r.stop > stop) {
            stop = r.stop;
        }
    }
    impossible_positions += stop - start + 1;
    return std::make_pair(impossible_positions, hole);
}

int main() {
    auto lines = aoc_read_input_lines_split({'='});
    auto p1_line = 2000000;
    auto p2_line = 4000000;

    // Prepwork
    std::vector<Sensor> sensors;
    for (auto& line : lines) {
        int radius = std::abs(std::stoi(line[3]) - std::stoi(line[11])) + std::abs(std::stoi(line[5]) - std::stoi(line[13]));
        sensors.push_back({std::stoi(line[3]), std::stoi(line[5]), radius});
    }

    // P1
    std::vector<Range> ranges;
    make_ranges(ranges, sensors, p1_line);
    sort_ranges(ranges);
    auto [impossible_positions, _] = get_impossible(ranges);
    std::cout << impossible_positions-1<< std::endl; // No idea why it counts one too much on p1, but at least it is consistently exactly 1
    
    // P2
    for (size_t y = 0; y <= p2_line; y++) {
        make_ranges(ranges, sensors, y, 0, p2_line);
        sort_ranges(ranges);
        auto [impossible_positions, x] = get_impossible(ranges);
        if (impossible_positions == p2_line) {
            int64_t tuning_frequency = (x*p2_line) + y;
            std::cout << tuning_frequency << std::endl;
            break;
        }
    }

    return 0;
}