#include "../common/filereader.h"

#include <memory>
#include <unordered_map>
#include <set>
#include <cmath>

class Node;

struct Edge {
    int weight;
    std::shared_ptr<Node> dest;
};

class Node {
    public:
        uint64_t flowrate = 0;
        std::vector<Edge> edges;
        std::string name;
        bool valve_is_opened = false;
        Node() = default;
        Node(std::string name) {this->name = name;}

};

std::unordered_map<std::string, std::shared_ptr<Node>> node_map;
std::shared_ptr<Node> get_node(std::string name) {
    if (node_map.find(name) == node_map.end())
        node_map[name] = std::make_shared<Node>(name);
    return node_map[name];
}



uint64_t potential_release(std::shared_ptr<Node> node, uint64_t minutes_left, std::set<std::shared_ptr<Node>>& opened_nodes) {
    uint64_t flowrate = node->flowrate * minutes_left;
    std::vector<uint64_t> releases = {flowrate};
    opened_nodes.insert(node);
    // Search
    for (auto& edge : node->edges) {
        if (opened_nodes.find(edge.dest) == opened_nodes.end() && minutes_left > edge.weight+1) {
            auto max_flow = potential_release(edge.dest, minutes_left - edge.weight - 1, opened_nodes);
            releases.push_back(max_flow + flowrate);
        }
    }
    // Sort and return
    opened_nodes.erase(node);
    std::sort(releases.begin(), releases.end());
    return releases.back();
}

void bfs(std::unordered_map<std::shared_ptr<Node>, int>& bfs_weights, std::shared_ptr<Node> node, std::set<std::shared_ptr<Node>> useful_nodes) {
    std::set<std::shared_ptr<Node>> next_layer = {node};
    std::set<std::shared_ptr<Node>> current_layer;
    std::set<std::shared_ptr<Node>> explored;
    int depth = 0;
    while (next_layer.size()) {
        // Load new layer of nodes to explore
        current_layer = next_layer;
        next_layer.clear();
        // Loop over current layer
        for (auto& current : current_layer) {
            explored.insert(current);
            if (useful_nodes.find(current) != useful_nodes.end()) {
                bfs_weights[current] = depth;
                if (bfs_weights.size() == useful_nodes.size()) return;
            }
            // Iterate over edges
            for (auto& edge : current->edges) {
                // Have not seen this node before
                if (explored.find(edge.dest) == explored.end() &&
                    current_layer.find(edge.dest) == current_layer.end()) {
                    // Add to next layer
                    next_layer.insert(edge.dest);
                }
            }
        }
        depth++;
    }

}

std::set<std::shared_ptr<Node>> make_set(std::vector<std::shared_ptr<Node>>& inventory, uint16_t mask) {
    std::set<std::shared_ptr<Node>> result;
    for (size_t i = 0; i < inventory.size(); i++) {
        if (mask & 1) result.insert(inventory[i]);
        mask = mask >> 1;
    }
    return result;
}

std::set<std::shared_ptr<Node>> make_anti(std::vector<std::shared_ptr<Node>>& base, std::set<std::shared_ptr<Node>>& reduce_by) {
    std::set<std::shared_ptr<Node>> result;
    for (auto& valve : base)
        if (reduce_by.find(valve) == reduce_by.end()) result.insert(valve);
    return result;
}

int main(int argc, char** argv) {

    auto lines = aoc_read_input_lines_split({',', '='});

    // Make graph of valves
    std::set<std::shared_ptr<Node>> useful_nodes;
    for (auto& line : lines) {
        auto node = get_node(line[1]);
        node->flowrate = std::stoi(line[5]);
        if (node->flowrate > 0) useful_nodes.insert(node);
        for (size_t i = 10; i < line.size(); i++) {
            auto connected_node = get_node(line[i]);
            node->edges.push_back({1, connected_node});
        }
    }
    auto start = get_node("AA");
    useful_nodes.insert(start);

    // Find connections between useful valves
    std::unordered_map<std::shared_ptr<Node>, std::unordered_map<std::shared_ptr<Node>, int>> new_connections;
    for (auto& node : useful_nodes) {
        std::unordered_map<std::shared_ptr<Node>, int> bfs_weights;
        bfs(bfs_weights, node, useful_nodes);
        new_connections[node] = bfs_weights;
    }

    // Apply info of usefulness to make new graph with only useful valves
    for (auto& node : useful_nodes) {
        node->edges.clear();
        for (auto& edges : new_connections[node]) {
            if (node != edges.first)
                node->edges.push_back({edges.second, edges.first});
        }
    }

    // P1
    std::set<std::shared_ptr<Node>> opened;
    auto best = potential_release(start, 30, opened);
    std::cout << best << std::endl;


    // P2
    std::vector<std::shared_ptr<Node>> valves;
    for (auto& valve : useful_nodes)
        if (valve != start)
            valves.push_back(valve);


    uint64_t max_score = 0;
    for (size_t i = 0; i < std::pow(2, valves.size())-1; i++) {
        auto human = make_set(valves, i);
        auto elephant = make_anti(valves, human);
        auto human_best = potential_release(start, 26, elephant);
        auto elephant_best = potential_release(start, 26, human);
        auto sum = human_best + elephant_best;
        if (max_score < sum)
            max_score = sum;
    }
    std::cout << max_score << std::endl;

    return 0;
}
