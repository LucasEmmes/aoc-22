#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>

#include "basenode.hpp"
#include "graph_edge.hpp"
#include "filehandling.hpp"

namespace Pathfinder {

template <class IdentifierType>
class AStar {
    private:
        std::unordered_map<IdentifierType, std::shared_ptr<BaseNode>> node_map;
        std::unordered_map<std::shared_ptr<BaseNode>, bool> queue_map;
        std::unordered_map<std::shared_ptr<BaseNode>, IdentifierType>
            reverse_node_map;
        std::weak_ptr<BaseNode> solved_for = std::weak_ptr<BaseNode>();
        std::function<uint64_t(IdentifierType, IdentifierType)> heuristic = 
            [](IdentifierType, IdentifierType){return 0;};

        // Getters
        std::shared_ptr<BaseNode> get_node(IdentifierType& identifier);
        void read_path_recursively(std::vector<IdentifierType>& path,
            std::shared_ptr<BaseNode> node);

        // Other
        void reset_nodes();
        bool has_registered_node(IdentifierType& identifier);
        bool node_in_queue(std::shared_ptr<BaseNode> node);

    public:
        AStar() = default;
        ~AStar() = default;
        AStar(
            std::function<uint64_t(IdentifierType, IdentifierType)> heuristic);

        // Setup
        void set_heuristic(
            std::function<uint64_t(IdentifierType, IdentifierType)> heuristic);
        void make_graph_from_edges(
            std::vector<Graph_Edge<IdentifierType>>& edges,
            bool directed = true);
        void make_graph_from_csv(const std::string& filepath,
            bool directed = true);

        // Other
        bool search_for_path(IdentifierType start, IdentifierType goal);
        uint64_t get_cost();
        void get_path(std::vector<IdentifierType>& path);
        std::vector<IdentifierType> get_path();
};

/**
 * Constructor which sets the given function as heurstic
 * 
 * @param heuristic Function to calculate the heuristic value of a node
*/
template <class IdentifierType>
AStar<IdentifierType>::AStar(
    std::function<uint64_t(IdentifierType, IdentifierType)> heuristic) {
    this->heuristic = heuristic;
}

/**
 * Lets you set or chage the heuristic function used by A*
 * 
 * @param heuristic Function to calculate the heuristic value of a node 
*/
template <class IdentifierType>
void AStar<IdentifierType>::set_heuristic(
    std::function<uint64_t(IdentifierType, IdentifierType)> heuristic) {
    this->heuristic = heuristic;
}

/**
 * Retrieve the related node
 * 
 * @param identifier Identifier of the node to be retrieved
 * @return A shared pointer to the node if it was found, otherwise a nullptr
*/
template <class IdentifierType>
std::shared_ptr<BaseNode> AStar<IdentifierType>::get_node(
    IdentifierType& identifier) {
    if (!has_registered_node(identifier)) {
        std::shared_ptr<BaseNode> node = std::make_shared<BaseNode>();
        node->set_ptr(node);
        node_map[identifier] = node;
        reverse_node_map[node] = identifier;
    }
    return node_map.at(identifier);
}

/**
 * If a path was found, it will read it into the provided vector
 * 
 * @param path Vector to store the path inside of.
 * Nodes listed from start to goal
*/
template <class IdentifierType>
void AStar<IdentifierType>::read_path_recursively(
    std::vector<IdentifierType>& path, std::shared_ptr<BaseNode> node) {
    path.push_back(this->reverse_node_map[node]);
    if (node->get_best_parent().expired()) return;
    else read_path_recursively(path, node->get_best_parent().lock());
}

/**
 * Resets the nodes to default values.
 * Useful when re-using the same graph for different paths.
*/
template <class IdentifierType>
void AStar<IdentifierType>::reset_nodes() {
    for (auto& it : node_map)
        it.second->reset();
}

/**
 * Checks whether a node is in the map already
 * 
 * @param identifier The identifier of the node
 * @return true if the node has been registered, false otherwise
*/
template <class IdentifierType>
bool AStar<IdentifierType>::has_registered_node(IdentifierType& identifier) {
    return this->node_map.find(identifier) != this->node_map.end();
}

/**
 * Checks whether a node is currently in the queue
 * 
 * @param node The node to check
 * @return true if the node is in queue, false otherwise
*/
template <class IdentifierType>
bool AStar<IdentifierType>::node_in_queue(std::shared_ptr<BaseNode> node) {
    return this->queue_map.find(node) != this->queue_map.end();
}

/**
 * Reads in a list of edges to use as a graph for searchin
 * 
 * @param edges Vector of Graph_Edge elements
 * @param directed Treats edges as directed if true (default) otherwise not.
*/
template <class IdentifierType>
void AStar<IdentifierType>::make_graph_from_edges(
    std::vector<Graph_Edge<IdentifierType>>& edges,
    bool directed) {
    for (Graph_Edge<IdentifierType>& edge : edges) {
        IdentifierType id_start = edge.get_start();
        IdentifierType id_dest = edge.get_destination();
        std::shared_ptr<BaseNode> node_start = get_node(id_start);
        std::shared_ptr<BaseNode> node_dest = get_node(id_dest);
        node_start->add_edge(Outgoing_Edge(edge.get_cost(), node_dest));
        if (!directed)
            node_dest->add_edge(Outgoing_Edge(edge.get_cost(), node_start));
    }
}

/**
 * Reads a .csv file to use as graph for searching
 * Use the following format: "IdentifierType, IdentifierType, uint64_t"  
 * which represent "node_from, node_to, cost"
 * 
 * @param filepath The filepath to the .csv file
 * @param directed Treats edges as directed if true (default) otherwise not.
*/
template <class IdentifierType>
void AStar<IdentifierType>::make_graph_from_csv(const std::string& filepath,
    bool directed) {
    auto edges = read_edges_from_csv<IdentifierType>(filepath);
    make_graph_from_edges(edges, directed);
}

/**
 * Searches for the shortest path between start and goal
 * 
 * @param start Identifier for start node
 * @param goal Identifier for goal node
 * @return True if a path was found, otherwise false
*/
template <class IdentifierType>
bool AStar<IdentifierType>::search_for_path(IdentifierType start, 
    IdentifierType goal) {
    // Sanity check
    if (!has_registered_node(start) || !has_registered_node(goal)) return false;
    // Set up queue and nodes
    auto cmp = [](std::shared_ptr<BaseNode> lhs, std::shared_ptr<BaseNode> rhs)
        {return lhs->get_current_estimate() > rhs->get_current_estimate();};
    reset_nodes();
    std::shared_ptr<BaseNode> node_start = get_node(start);
    std::shared_ptr<BaseNode> node_goal = get_node(goal);
    node_start->set_as_start();
    std::shared_ptr<BaseNode> node_current;
    std::vector<std::shared_ptr<BaseNode>> queue = {node_start};
    std::vector<std::shared_ptr<BaseNode>> updated_nodes;
    queue_map[node_start] = true;

    // Make queue of all the nodes
    for (auto& it : node_map) {
        node_current = it.second;
        // Set heuristic of all nodes
        node_current->set_heuristic(heuristic(it.first, goal));
    }

    // Update loop
    while (queue.size() && queue.back() != node_goal) {
        node_current = queue.back();
        queue_map.erase(node_current);
        queue.pop_back();
        updated_nodes = {};
        // Get all the nodes which were updated with new paths
        node_current->update_edges(updated_nodes);
        for (auto& updated_node : updated_nodes)
            // Put them in the queue if they aren't already
            if (!node_in_queue(updated_node)) {
                queue.push_back(updated_node);
                queue_map[updated_node] = true;
            };
        std::sort(queue.begin(), queue.end(), cmp);
    }

    queue_map.clear();

    // Set the found-pointer appropriately and return whether we found something
    if (!node_goal->get_best_parent().expired()) {
        this->solved_for = node_goal;
        return true;
    }
    return false;
}

/**
 * This will retrieve the cost of recently found path
 * 
 * @return Cost of travelig the path
*/
template <class IdentifierType>
uint64_t AStar<IdentifierType>::get_cost() {
    if (!this->solved_for.expired())
        return this->solved_for.lock()->get_current_cost();
    else
        return UINT64_MAX;
}

/**
 * This will retrieve the shortest recently found path
 * 
 * @param path A vector containing all the steps from start to goal
*/
template <class IdentifierType>
void AStar<IdentifierType>::get_path(std::vector<IdentifierType>& path) {
    // If we don't have a path to give out, just return
    if (this->solved_for.expired()) return;
    // Otherwise, retrieve
    read_path_recursively(path, this->solved_for.lock());
    std::reverse(path.begin(), path.end());
}

/**
 * This will retrieve the shortest recently found path
 * 
 * @return A vector containing all the steps from start to goal
*/
template <class IdentifierType>
std::vector<IdentifierType> AStar<IdentifierType>::get_path() {
    // Create a vector, then simply call the other version of this function
    std::vector<IdentifierType> path;
    get_path(path);
    return path;
}

} // namespace Pathfinder

#endif