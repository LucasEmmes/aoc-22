#include "../common/filereader.h"

#include <memory>
#include <unordered_map>

class Node {
public:
    int flowrate = 0;
    std::vector<std::shared_ptr<Node>> edges;
    bool open = false;
};

int64_t get_max_flow(std::shared_ptr<Node>& node, int minutes_left) {
    if (minutes_left == 0) return 0;
    
    int64_t max_possible_release = 0;
    for (auto& con : node->edges) {
        // int64_t possible_release = get_max_flow(node);
    }
    
}

int main() {
    auto lines = aoc_read_input_lines_split({'=', ','});

    std::unordered_map<std::string, std::shared_ptr<Node>> node_map;

    for (auto& line : lines) {
        std::shared_ptr<Node> node = std::make_shared<Node>();
        node_map[line[1]] = node;
        node->flowrate = std::stoi(line[5]);

        for (size_t i = 10; i < line.size(); i++) {
            if (node_map.find(line[i]) == node_map.end()) {
                node_map[line[i]] = std::make_shared<Node>();
            }
            node->edges.push_back(node_map[line[i]]);
        }
    }

    return 0;
}